//
// (C) Copyright 2022 Intel Corporation.
//
// SPDX-License-Identifier: BSD-2-Clause-Patent
//

package raft

import (
	"context"
	"fmt"
	"sync"
	"sync/atomic"
	"time"

	"github.com/google/uuid"
	"github.com/pkg/errors"

	"github.com/daos-stack/daos/src/control/logging"
)

type (
	PoolLock struct {
		id       uuid.UUID
		poolUUID uuid.UUID
		takenAt  time.Time
		refCount int32
		release  func()
	}

	poolLockMap struct {
		sync.RWMutex
		locks map[uuid.UUID]*PoolLock
		log   logging.DebugLogger
	}

	ctxKey string
)

const (
	poolLockKey ctxKey = "poolLock"
)

var (
	errNoCtxLock = errors.New("no pool lock in context")
)

func getCtxLock(ctx context.Context) (*PoolLock, error) {
	if ctx == nil {
		return nil, errors.New("nil context in getCtxLock()")
	}

	lock, ok := ctx.Value(poolLockKey).(*PoolLock)
	if !ok {
		return nil, errNoCtxLock
	}

	return lock, nil
}

// IsPoolLockError returns true if the supplied error is
// an instance of poolLockError.
func IsPoolLockError(err error) bool {
	_, ok := errors.Cause(err).(*poolLockError)
	return ok
}

type poolLockError struct {
	lock *PoolLock
}

func (e *poolLockError) Error() string {
	return fmt.Sprintf("pool %s locked @ %s (lock id: %s)", e.lock.poolUUID, e.lock.takenAt, e.lock.id)
}

func (pl *PoolLock) WithContext(ctx context.Context) context.Context {
	lock, err := getCtxLock(ctx)
	if err != nil && err != errNoCtxLock {
		return ctx
	} else if lock != nil {
		if lock.id != pl.id {
			return ctx
		}
	}

	return context.WithValue(ctx, poolLockKey, pl)
}

func (pl *PoolLock) addRef() {
	atomic.AddInt32(&pl.refCount, 1)
}

func (pl *PoolLock) decRef() {
	atomic.AddInt32(&pl.refCount, -1)
}

func (pl *PoolLock) Release() {
	pl.decRef()
	if atomic.LoadInt32(&pl.refCount) > 0 {
		return
	}
	pl.release()
}

func (plm *poolLockMap) take(poolUUID uuid.UUID) (*PoolLock, error) {
	plm.Lock()
	defer plm.Unlock()

	if plm.locks == nil {
		plm.locks = make(map[uuid.UUID]*PoolLock)
	}

	if lock, exists := plm.locks[poolUUID]; exists {
		return nil, &poolLockError{lock}
	}

	lock := &PoolLock{
		id:       uuid.New(),
		poolUUID: poolUUID,
		takenAt:  time.Now(),
		release:  func() { plm.release(poolUUID) },
	}
	lock.addRef()
	plm.locks[poolUUID] = lock

	plm.log.Debugf("%s: lock taken (id: %s)", dbgUuidStr(poolUUID), dbgUuidStr(lock.id))
	return lock, nil
}

func (plm *poolLockMap) release(poolUUID uuid.UUID) {
	plm.Lock()
	defer plm.Unlock()

	plm.log.Debugf("%s: lock released", dbgUuidStr(poolUUID))
	delete(plm.locks, poolUUID)
}

func (plm *poolLockMap) checkLockCtx(ctx context.Context) error {
	lock, err := getCtxLock(ctx)
	if err != nil {
		return err
	}

	return plm.checkLock(lock)
}

func (plm *poolLockMap) checkLock(lock *PoolLock) error {
	if lock == nil {
		return errors.New("nil pool lock in checkLock()")
	}
	plm.RLock()
	defer plm.RUnlock()

	if pl, exists := plm.locks[lock.poolUUID]; exists {
		if lock.id != pl.id {
			return &poolLockError{lock}
		}
	} else {
		return errors.Errorf("pool %s: lock not found", lock.poolUUID)
	}

	return nil
}
