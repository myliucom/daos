//
// (C) Copyright 2018-2022 Intel Corporation.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

package server

import (
	"context"
	mgmtpb "github.com/daos-stack/daos/src/control/common/proto/mgmt"
	"github.com/google/uuid"
	"github.com/pkg/errors"
	"google.golang.org/protobuf/proto"

	sharedpb "github.com/daos-stack/daos/src/control/common/proto/shared"
	srvpb "github.com/daos-stack/daos/src/control/common/proto/srv"
	"github.com/daos-stack/daos/src/control/drpc"
	"github.com/daos-stack/daos/src/control/events"
	"github.com/daos-stack/daos/src/control/lib/daos"
	"github.com/daos-stack/daos/src/control/lib/ranklist"
	"github.com/daos-stack/daos/src/control/logging"
	"github.com/daos-stack/daos/src/control/system"
)

// mgmtModule represents the daos_server mgmt dRPC module. It sends dRPCs to
// the daos_engine (src/engine) but doesn't receive.
type mgmtModule struct{}

// newMgmtModule creates a new management module and returns its reference.
func newMgmtModule() *mgmtModule {
	return &mgmtModule{}
}

// HandleCall is the handler for calls to the mgmtModule
func (mod *mgmtModule) HandleCall(_ context.Context, session *drpc.Session, method drpc.Method, req []byte) ([]byte, error) {
	return nil, drpc.UnknownMethodFailure()
}

// ID will return Mgmt module ID
func (mod *mgmtModule) ID() drpc.ModuleID {
	return drpc.ModuleMgmt
}

// poolResolver defines an interface to be implemented by
// something that can resolve a pool ID into a PoolService.
type poolResolver interface {
	FindPoolServiceByLabel(string) (*system.PoolService, error)
	FindPoolServiceByUUID(uuid.UUID) (*system.PoolService, error)
}

// srvModule represents the daos_server dRPC module. It handles dRPCs sent by
// the daos_engine (src/engine).
type srvModule struct {
	log     logging.Logger
	sysdb   poolResolver
	engines []Engine
	events  *events.PubSub
	mgmtSvc *mgmtSvc // TODO - Could create interface like poolResolver??
}

// newSrvModule creates a new srv module references to the system database,
// resident EngineInstances and event publish subscribe reference.
func newSrvModule(log logging.Logger, sysdb poolResolver, engines []Engine, events *events.PubSub, svc *mgmtSvc) *srvModule {
	return &srvModule{
		log:     log,
		sysdb:   sysdb,
		engines: engines,
		events:  events,
		mgmtSvc: svc,
	}
}

// HandleCall is the handler for calls to the srvModule.
func (mod *srvModule) HandleCall(_ context.Context, session *drpc.Session, method drpc.Method, req []byte) ([]byte, error) {
	switch method {
	case drpc.MethodNotifyReady:
		return nil, mod.handleNotifyReady(req)
	case drpc.MethodBIOError:
		return nil, mod.handleBioErr(req)
	case drpc.MethodGetPoolServiceRanks:
		return mod.handleGetPoolServiceRanks(req)
	case drpc.MethodPoolFindByLabel:
		return mod.handlePoolFindByLabel(req)
	case drpc.MethodClusterEvent:
		return mod.handleClusterEvent(req)
	case drpc.MethodSystemGetProp:
		return mod.handleSystemProperties(req)
	default:
		return nil, drpc.UnknownMethodFailure()
	}
}

// ID will return SRV module ID
func (mod *srvModule) ID() drpc.ModuleID {
	return drpc.ModuleSrv
}

func (mod *srvModule) handleGetPoolServiceRanks(reqb []byte) ([]byte, error) {
	req := new(srvpb.GetPoolSvcReq)
	if err := proto.Unmarshal(reqb, req); err != nil {
		return nil, drpc.UnmarshalingPayloadFailure()
	}

	uuid, err := uuid.Parse(req.GetUuid())
	if err != nil {
		return nil, errors.Wrapf(err, "invalid pool uuid %q", uuid)
	}

	mod.log.Debugf("handling GetPoolSvcReq: %+v", req)

	resp := new(srvpb.GetPoolSvcResp)

	ps, err := mod.sysdb.FindPoolServiceByUUID(uuid)
	if err != nil {
		resp.Status = int32(daos.Nonexistent)
		mod.log.Debugf("GetPoolSvcResp: %+v", resp)
		return proto.Marshal(resp)
	}

	resp.Svcreps = ranklist.RanksToUint32(ps.Replicas)

	mod.log.Debugf("GetPoolSvcResp: %+v", resp)

	return proto.Marshal(resp)
}

func (mod *srvModule) handlePoolFindByLabel(reqb []byte) ([]byte, error) {
	req := new(srvpb.PoolFindByLabelReq)
	if err := proto.Unmarshal(reqb, req); err != nil {
		return nil, drpc.UnmarshalingPayloadFailure()
	}

	mod.log.Debugf("handling PoolFindByLabel: %+v", req)

	resp := new(srvpb.PoolFindByLabelResp)

	ps, err := mod.sysdb.FindPoolServiceByLabel(req.GetLabel())
	if err != nil {
		resp.Status = int32(daos.Nonexistent)
		mod.log.Debugf("PoolFindByLabelResp: %+v", resp)
		return proto.Marshal(resp)
	}

	resp.Svcreps = ranklist.RanksToUint32(ps.Replicas)
	resp.Uuid = ps.PoolUUID.String()
	mod.log.Debugf("GetPoolSvcResp: %+v", resp)

	return proto.Marshal(resp)
}

func (mod *srvModule) handleNotifyReady(reqb []byte) error {
	req := &srvpb.NotifyReadyReq{}
	if err := proto.Unmarshal(reqb, req); err != nil {
		return drpc.UnmarshalingPayloadFailure()
	}

	if req.InstanceIdx >= uint32(len(mod.engines)) {
		return errors.Errorf("instance index %v is out of range (%v instances)",
			req.InstanceIdx, len(mod.engines))
	}

	if err := checkDrpcClientSocketPath(req.DrpcListenerSock); err != nil {
		return errors.Wrap(err, "check NotifyReady request socket path")
	}

	mod.engines[req.InstanceIdx].NotifyDrpcReady(req)

	return nil
}

func (mod *srvModule) handleBioErr(reqb []byte) error {
	req := &srvpb.BioErrorReq{}
	if err := proto.Unmarshal(reqb, req); err != nil {
		return errors.Wrap(err, "unmarshal BioError request")
	}

	if req.InstanceIdx >= uint32(len(mod.engines)) {
		return errors.Errorf("instance index %v is out of range (%v instances)",
			req.InstanceIdx, len(mod.engines))
	}

	if err := checkDrpcClientSocketPath(req.DrpcListenerSock); err != nil {
		return errors.Wrap(err, "check BioErr request socket path")
	}

	mod.engines[req.InstanceIdx].BioErrorNotify(req)

	return nil
}

func (mod *srvModule) handleClusterEvent(reqb []byte) ([]byte, error) {
	req := new(sharedpb.ClusterEventReq)
	if err := proto.Unmarshal(reqb, req); err != nil {
		return nil, drpc.UnmarshalingPayloadFailure()
	}

	resp, err := mod.events.HandleClusterEvent(req, false)
	if err != nil {
		return nil, errors.Wrapf(err, "handle cluster event %+v", req)
	}

	return proto.Marshal(resp)
}

func (mod *srvModule) handleSystemProperties(reqb []byte) ([]byte, error) {
	mod.log.Error("Bang")
	req := new(srvpb.GetSystemPropReq)
	if err := proto.Unmarshal(reqb, req); err != nil {
		return nil, drpc.UnmarshalingPayloadFailure()
	}
	mod.log.Errorf("handling GetSystemProperties: %+v", req)

	mod.log.Errorf("System Property Count: %d", len(mod.mgmtSvc.systemProps))
	propReq := mgmtpb.SystemGetPropReq{
		Sys:  "daos_server",
		Keys: nil,
	}
	if resp2, err := mod.mgmtSvc.SystemGetProp(nil, &propReq); err != nil {
		mod.log.Errorf("Got error: %s", err)
	} else {
		mod.log.Errorf("Got response: %s", resp2)
	}

	resp := new(srvpb.GetSystemPropResp)

	resp.Status = 0
	resp.ReturnText = "Bang"
	mod.log.Errorf("GetSystemProperties: %+v", resp)

	return proto.Marshal(resp)
}
