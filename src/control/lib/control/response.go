//
// (C) Copyright 2020-2022 Intel Corporation.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

package control

import (
	"encoding/json"
	"sort"
	"strings"

	"github.com/dustin/go-humanize/english"
	"github.com/pkg/errors"
	"google.golang.org/protobuf/proto"

	"github.com/daos-stack/daos/src/control/common/proto/convert"
	ctlpb "github.com/daos-stack/daos/src/control/common/proto/ctl"
	"github.com/daos-stack/daos/src/control/lib/hostlist"
)

var (
	errNoMsResponse = errors.New("response did not contain a management service response")
)

type (
	// HostResponse contains a single host's response to an unary RPC, or
	// an error if the host was unable to respond successfully.
	HostResponse struct {
		Addr    string
		Error   error
		Message proto.Message
	}

	// HostResponseChan defines a channel of *HostResponse items returned
	// from asynchronous unary RPC invokers.
	HostResponseChan chan *HostResponse

	// HostResponseReport defines the function signature for a callback
	// invoked when a host response is received.
	HostResponseReportFn func(*HostResponse)
)

// HostErrorsResp is a response type containing a HostErrorsMap.
type HostErrorsResp struct {
	HostErrors HostErrorsMap `json:"host_errors"`
}

func (her *HostErrorsResp) addHostError(hostAddr string, hostErr error) error {
	if her.HostErrors == nil {
		her.HostErrors = make(HostErrorsMap)
	}
	return her.HostErrors.Add(hostAddr, hostErr)
}

// GetHostErrors retrieves a HostErrorsMap from a response type.
func (her *HostErrorsResp) GetHostErrors() HostErrorsMap {
	return her.HostErrors
}

// Errors returns an error containing brief description of errors in map.
func (her *HostErrorsResp) Errors() error {
	if len(her.HostErrors) > 0 {
		erroredHosts := make(map[string]bool)
		for _, hes := range her.HostErrors {
			hostsInSet := strings.Split(hes.HostSet.DerangedString(), ",")
			for _, host := range hostsInSet {
				if _, exists := erroredHosts[host]; !exists {
					erroredHosts[host] = true
				}
			}
		}

		return errors.Errorf("%s had errors",
			english.Plural(len(erroredHosts), "host", "hosts"))
	}
	return nil
}

// HostErrorSet preserves the original hostError used
// to create the map key.
type HostErrorSet struct {
	HostSet   *hostlist.HostSet
	HostError error
}

// HostErrorsMap provides a mapping from error strings to a set of
// hosts to which the error applies.
type HostErrorsMap map[string]*HostErrorSet

// MarshalJSON implements a custom marshaller to include
// the hostset as a ranged string.
func (hem HostErrorsMap) MarshalJSON() ([]byte, error) {
	out := make(map[string]string)
	for k, hes := range hem {
		// quick sanity check to prevent panics
		if hes == nil || hes.HostSet == nil {
			return nil, errors.New("nil hostErrorSet or hostSet")
		}
		out[k] = hes.HostSet.RangedString()
	}
	return json.Marshal(out)
}

// Add creates or updates the err/addr keyval pair.
func (hem HostErrorsMap) Add(hostAddr string, hostErr error) (err error) {
	if hostErr == nil {
		return nil
	}

	errStr := hostErr.Error() // stringify the error as map key
	if _, exists := hem[errStr]; !exists {
		hes := &HostErrorSet{
			HostError: hostErr,
		}
		hes.HostSet, err = hostlist.CreateSet(hostAddr)
		if err == nil {
			hem[errStr] = hes
		}
		return
	}
	_, err = hem[errStr].HostSet.Insert(hostAddr)
	return
}

// Keys returns a stable sorted slice of the errors map keys.
func (hem HostErrorsMap) Keys() []string {
	setToKeys := make(map[string]map[string]struct{})
	for errStr, hes := range hem {
		rs := hes.HostSet.RangedString()
		if _, exists := setToKeys[rs]; !exists {
			setToKeys[rs] = make(map[string]struct{})
		}
		setToKeys[rs][errStr] = struct{}{}
	}

	sets := make([]string, 0, len(hem))
	for set := range setToKeys {
		sets = append(sets, set)
	}
	sort.Strings(sets)

	keys := make([]string, 0, len(hem))
	for _, set := range sets {
		setKeys := make([]string, 0, len(setToKeys[set]))
		for key := range setToKeys[set] {
			setKeys = append(setKeys, key)
		}
		sort.Strings(setKeys)
		keys = append(keys, setKeys...)
	}
	return keys
}

// UnaryResponse contains a slice of *HostResponse items returned
// from synchronous unary RPC invokers.
type UnaryResponse struct {
	Responses []*HostResponse
	fromMS    bool
}

// getMSResponse is a helper method to return the MS response
// message from a UnaryResponse.
func (ur *UnaryResponse) getMSResponse() (proto.Message, error) {
	if ur == nil {
		return nil, errors.Errorf("nil %T", ur)
	}

	if !ur.fromMS {
		return nil, errors.New("response did not come from management service")
	}

	if len(ur.Responses) == 0 {
		return nil, errNoMsResponse
	}

	// As we may have sent the request to multiple MS replicas, just pick
	// through the responses to find the one that succeeded. If none succeeded,
	// return the error from the last response.
	var msResp *HostResponse
	for _, msResp = range ur.Responses {
		if msResp.Error != nil || msResp.Message == nil {
			continue
		}

		break
	}

	if msResp == nil {
		return nil, errNoMsResponse
	}

	if msResp.Error != nil {
		return nil, msResp.Error
	}

	if msResp.Message == nil {
		return nil, errors.New("management service response message was nil")
	}

	return msResp.Message, nil
}

// convertMSResponse is a helper function to extract the MS response
// message from a generic UnaryResponse. The out parameter must be
// a reference to a compatible concrete type (e.g. PoolQueryResp).
func convertMSResponse(ur *UnaryResponse, out interface{}) error {
	msResp, err := ur.getMSResponse()
	if err != nil {
		if IsConnErr(err) {
			return errMSConnectionFailure
		}
		return err
	}

	return convert.Types(msResp, out)
}

// ctlStateToErr is a helper function for turning an
// unsuccessful control response state into an error.
func ctlStateToErr(state *ctlpb.ResponseState) error {
	if state.GetStatus() == ctlpb.ResponseStatus_CTL_SUCCESS {
		return nil
	}

	if errMsg := state.GetError(); errMsg != "" {
		return errors.New(errMsg)
	}
	if infoMsg := state.GetInfo(); infoMsg != "" {
		return errors.Errorf("%s: %s", state.GetStatus(), infoMsg)
	}
	return errors.Errorf("%s: unknown error", state.GetStatus())
}
