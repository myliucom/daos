//
// (C) Copyright 2020-2022 Intel Corporation.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

package server

import (
	"context"

	"github.com/pkg/errors"

	"github.com/daos-stack/daos/src/control/common/proto/convert"
	ctlpb "github.com/daos-stack/daos/src/control/common/proto/ctl"
	"github.com/daos-stack/daos/src/control/server/storage"
)

// FirmwareQuery implements the method defined for the control service if
// firmware management is enabled for this build.
//
// It fetches information about the device firmware on this server based on the
// caller's request parameters. It can fetch firmware information for NVMe, SCM,
// or both.
func (svc *ControlService) FirmwareQuery(parent context.Context, pbReq *ctlpb.FirmwareQueryReq) (*ctlpb.FirmwareQueryResp, error) {
	svc.log.Debug("received FirmwareQuery RPC")

	pbResp := new(ctlpb.FirmwareQueryResp)

	if pbReq.QueryScm {
		scmResults, err := svc.querySCMFirmware(pbReq)
		if err != nil {
			return nil, err
		}
		pbResp.ScmResults = scmResults
	}

	if pbReq.QueryNvme {
		nvmeResults, err := svc.queryNVMeFirmware(pbReq)
		if err != nil {
			return nil, err
		}
		pbResp.NvmeResults = nvmeResults
	}

	svc.log.Debug("responding to FirmwareQuery RPC")
	return pbResp, nil
}

func (svc *ControlService) querySCMFirmware(pbReq *ctlpb.FirmwareQueryReq) ([]*ctlpb.ScmFirmwareQueryResp, error) {
	queryResp, err := svc.storage.QueryScmFirmware(storage.ScmFirmwareQueryRequest{
		FirmwareRev: pbReq.FirmwareRev,
		ModelID:     pbReq.ModelID,
		DeviceUIDs:  pbReq.DeviceIDs,
	})
	if err != nil {
		return nil, err
	}

	scmResults := make([]*ctlpb.ScmFirmwareQueryResp, 0, len(queryResp.Results))
	for _, res := range queryResp.Results {
		pbResult := &ctlpb.ScmFirmwareQueryResp{}
		if err := convert.Types(res.Module, &pbResult.Module); err != nil {
			return nil, errors.Wrap(err, "unable to convert SCM module")
		}
		if res.Info != nil {
			pbResult.ActiveVersion = res.Info.ActiveVersion
			pbResult.StagedVersion = res.Info.StagedVersion
			pbResult.ImageMaxSizeBytes = res.Info.ImageMaxSizeBytes
			pbResult.UpdateStatus = uint32(res.Info.UpdateStatus)
		}
		pbResult.Error = res.Error
		scmResults = append(scmResults, pbResult)
	}

	return scmResults, nil
}

func (svc *ControlService) queryNVMeFirmware(pbReq *ctlpb.FirmwareQueryReq) ([]*ctlpb.NvmeFirmwareQueryResp, error) {
	queryResp, err := svc.storage.QueryBdevFirmware(storage.NVMeFirmwareQueryRequest{
		FirmwareRev: pbReq.FirmwareRev,
		ModelID:     pbReq.ModelID,
		DeviceAddrs: pbReq.DeviceIDs,
	})
	if err != nil {
		return nil, err
	}

	nvmeResults := make([]*ctlpb.NvmeFirmwareQueryResp, 0, len(queryResp.Results))
	for _, res := range queryResp.Results {
		pbResult := &ctlpb.NvmeFirmwareQueryResp{}
		if err := convert.Types(res.Device, &pbResult.Device); err != nil {
			return nil, errors.Wrap(err, "unable to convert NVMe controller")
		}

		nvmeResults = append(nvmeResults, pbResult)
	}

	return nvmeResults, nil
}

// FirmwareUpdate implements the method defined for the control service if
// firmware management is enabled for this build.
//
// It updates the firmware on the storage devices of the specified type.
func (svc *ControlService) FirmwareUpdate(parent context.Context, pbReq *ctlpb.FirmwareUpdateReq) (*ctlpb.FirmwareUpdateResp, error) {
	svc.log.Debug("received FirmwareUpdate RPC")

	instances := svc.harness.Instances()
	for _, srv := range instances {
		if srv.IsStarted() {
			rank, err := srv.GetRank()
			if err != nil {
				return nil, errors.New("unidentified server rank is running")
			}
			return nil, FaultInstancesNotStopped("firmware update", rank)
		}
	}

	pbResp := new(ctlpb.FirmwareUpdateResp)
	var err error
	switch pbReq.Type {
	case ctlpb.FirmwareUpdateReq_SCM:
		err = svc.updateSCM(pbReq, pbResp)
	case ctlpb.FirmwareUpdateReq_NVMe:
		err = svc.updateNVMe(pbReq, pbResp)
	default:
		err = errors.New("unrecognized device type")
	}

	if err != nil {
		return nil, err
	}

	svc.log.Debug("responding to FirmwareUpdate RPC")
	return pbResp, nil
}

func (svc *ControlService) updateSCM(pbReq *ctlpb.FirmwareUpdateReq, pbResp *ctlpb.FirmwareUpdateResp) error {
	updateResp, err := svc.storage.UpdateScmFirmware(storage.ScmFirmwareUpdateRequest{
		FirmwarePath: pbReq.FirmwarePath,
		FirmwareRev:  pbReq.FirmwareRev,
		ModelID:      pbReq.ModelID,
		DeviceUIDs:   pbReq.DeviceIDs,
	})
	if err != nil {
		return err
	}

	pbResp.ScmResults = make([]*ctlpb.ScmFirmwareUpdateResp, 0, len(updateResp.Results))
	for _, res := range updateResp.Results {
		pbRes := &ctlpb.ScmFirmwareUpdateResp{}
		if err := convert.Types(res, pbRes); err != nil {
			return err
		}
		pbRes.Error = res.Error
		pbResp.ScmResults = append(pbResp.ScmResults, pbRes)
	}
	return nil
}

func (svc *ControlService) updateNVMe(pbReq *ctlpb.FirmwareUpdateReq, pbResp *ctlpb.FirmwareUpdateResp) error {
	updateResp, err := svc.storage.UpdateBdevFirmware(storage.NVMeFirmwareUpdateRequest{
		FirmwarePath: pbReq.FirmwarePath,
		FirmwareRev:  pbReq.FirmwareRev,
		ModelID:      pbReq.ModelID,
		DeviceAddrs:  pbReq.DeviceIDs,
	})
	if err != nil {
		return err
	}

	pbResp.NvmeResults = make([]*ctlpb.NvmeFirmwareUpdateResp, 0, len(updateResp.Results))
	for _, res := range updateResp.Results {
		pbRes := &ctlpb.NvmeFirmwareUpdateResp{
			PciAddr: res.Device.PciAddr,
			Error:   res.Error,
		}
		pbResp.NvmeResults = append(pbResp.NvmeResults, pbRes)
	}
	return nil
}
