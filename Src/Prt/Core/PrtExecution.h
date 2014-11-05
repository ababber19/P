#ifndef PRT_EXECUTION_H
#define PRT_EXECUTION_H

#include "Prt.h"

#ifdef __cplusplus
extern "C"{
#endif

//
// Max call stack size of each machine
//
#define PRT_MAX_CALL_DEPTH 16 

//
// Initial length of the event queue for each machine
//
#define PRT_QUEUE_LEN_DEFAULT 64

typedef struct PRT_PROCESS_PRIV {
	PRT_GUID				guid;
	PRT_PROGRAMDECL			*program;
	PRT_ERROR_FUN	        errorHandler;
	PRT_LOG_FUN				logHandler;
	PRT_RECURSIVE_MUTEX		processLock;
	PRT_UINT32				numMachines;
	PRT_UINT32				machineCount;
	PRT_SM_CONTEXT			**machines;
} PRT_PROCESS_PRIV;

typedef enum PRT_STATECONTROL
{
	//
	// Execute the entry function
	//
	PrtStateEntry,
	//
	// Execute the action corresponding to currentEvent
	//
	PrtStateAction,
	//
	// Dequeue an event
	//
	PrtDequeue
} PRT_STATECONTROL;

typedef enum PRT_LASTOPERATION
{
	ReturnStatement,
	PopStatement,
	RaiseStatement,
	PushStatement
} PRT_LASTOPERATION;

typedef struct PRT_EVENT
{
	PRT_VALUE *trigger;
	PRT_VALUE *payload;
} PRT_EVENT;

typedef struct PRT_EVENTQUEUE
{
	PRT_UINT32		 eventsSize;
	PRT_EVENT		*events;
	PRT_UINT32		 headIndex;
	PRT_UINT32		 tailIndex;
	PRT_UINT32		 size;
} PRT_EVENTQUEUE;

typedef struct PRT_STACKSTATE_INFO
{
	PRT_UINT32			stateIndex;
	PRT_EVENT			currEvent;
	PRT_UINT16			returnTo;
	PRT_STATECONTROL	stateControl;
	PRT_UINT32*			inheritedDeferredSetCompact;
	PRT_UINT32*			inheritedActionsSetCompact;
} PRT_STACKSTATE_INFO;

typedef struct PRT_STATESTACK
{
	PRT_STACKSTATE_INFO statesStack[PRT_MAX_CALL_DEPTH];
	PRT_UINT16			length;
} PRT_STATESTACK;

typedef struct PRT_SM_CONTEXT_PRIV {
	PRT_PROCESS		    *process;
	PRT_UINT32			instanceOf;
	PRT_VALUE			*id;  
	void				*extContext;
	PRT_BOOLEAN			isModel;
	PRT_VALUE			**varValues;
	PRT_EVENT			currentEvent;
	PRT_RECURSIVE_MUTEX stateMachineLock;
	PRT_BOOLEAN			isRunning;
	PRT_BOOLEAN			isHalted;
	PRT_UINT32			currentState;
	PRT_STATESTACK		callStack;
	PRT_EVENTQUEUE		eventQueue;
	PRT_LASTOPERATION	lastOperation;
	PRT_STATECONTROL	stateControl;
	PRT_UINT16			returnTo;
	PRT_UINT32*			inheritedDeferredSetCompact;
	PRT_UINT32*			currentDeferredSetCompact;
	PRT_UINT32*			inheritedActionsSetCompact;
	PRT_UINT32*			currentActionsSetCompact;
} PRT_SM_CONTEXT_PRIV;

PRT_SM_CONTEXT_PRIV *
PrtMkMachinePrivate(
_Inout_  PRT_PROCESS_PRIV		*process,
_In_  PRT_UINT32				instanceOf,
_In_  PRT_VALUE					*payload
);

void
PrtSendPrivate(
_Inout_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_VALUE					*event,
_In_ PRT_VALUE					*payload
);

PRT_API void PRT_CALL_CONV
PrtRaise(
_Inout_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_VALUE					*event,
_In_ PRT_VALUE					*payload
);

PRT_API void PRT_CALL_CONV
PrtPush(
_Inout_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_UINT32					stateIndex
);

void
PrtPushState(
_Inout_ PRT_SM_CONTEXT_PRIV	*context,
_In_	PRT_UINT32			stateIndex,
_In_	PRT_BOOLEAN			isPushStatement
);

PRT_API void PRT_CALL_CONV
PrtPop(
_Inout_ PRT_SM_CONTEXT_PRIV		*context
);

void
PrtPopState(
_Inout_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_BOOLEAN				isPopStatement
);

FORCEINLINE
void
PrtRunExitFunction(
_In_ PRT_SM_CONTEXT_PRIV			*context,
_In_ PRT_UINT32						transIndex
);

void
PrtRunStateMachine(
_Inout_ PRT_SM_CONTEXT_PRIV	    *context,
_In_ PRT_BOOLEAN				doDequeue
);

PRT_UINT32
PrtFindTransition(
_In_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_UINT32					eventIndex
);

void
PrtTakeTransition(
_Inout_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_UINT32					eventIndex
);

PRT_BOOLEAN
PrtDequeueEvent(
_Inout_ PRT_SM_CONTEXT_PRIV	*context
);

FORCEINLINE
PRT_STATEDECL *
PrtGetCurrentStateDecl(
_In_ PRT_SM_CONTEXT_PRIV			*context
);

PRT_TYPE*
PrtGetPayloadType(
_In_ PRT_SM_CONTEXT_PRIV	*context,
_In_ PRT_VALUE				*event
);

FORCEINLINE
PRT_UINT16
PrtGetPackSize(
_In_ PRT_SM_CONTEXT_PRIV			*context
);

FORCEINLINE
PRT_SM_FUN
PrtGetEntryFunction(
_In_ PRT_SM_CONTEXT_PRIV		*context
);

FORCEINLINE
PRT_SM_FUN
PrtGetExitFunction(
_In_ PRT_SM_CONTEXT_PRIV		*context
);

FORCEINLINE
PRT_DODECL*
PrtGetAction(
_In_ PRT_SM_CONTEXT_PRIV		*context
);

FORCEINLINE
PRT_UINT32*
PrtGetDeferredPacked(
_In_ PRT_SM_CONTEXT_PRIV	*context,
_In_ PRT_UINT32				stateIndex
);

FORCEINLINE
PRT_UINT32*
PrtGetActionsPacked(
_In_ PRT_SM_CONTEXT_PRIV	*context,
_In_ PRT_UINT32				stateIndex
);

FORCEINLINE
PRT_UINT32*
PrtGetTransitionsPacked(
_In_ PRT_SM_CONTEXT_PRIV	*context,
_In_ PRT_UINT32				stateIndex
);

FORCEINLINE
PRT_TRANSDECL*
PrtGetTransitionTable(
_In_ PRT_SM_CONTEXT_PRIV	*context,
_In_ PRT_UINT32				stateIndex,
_Out_ PRT_UINT32			*nTransitions
);

PRT_BOOLEAN
PrtAreGuidsEqual(
_In_ PRT_GUID guid1,
_In_ PRT_GUID guid2
);

PRT_BOOLEAN
PrtIsEventMaxInstanceExceeded(
_In_ PRT_EVENTQUEUE			*queue,
_In_ PRT_UINT32				eventIndex,
_In_ PRT_UINT32				maxInstances
);

FORCEINLINE
PRT_BOOLEAN
PrtStateHasDefaultTransitionOrAction(
_In_ PRT_SM_CONTEXT_PRIV			*context
);

FORCEINLINE
PRT_BOOLEAN
PrtIsSpecialEvent(
_In_ PRT_VALUE * event
);

FORCEINLINE
PRT_BOOLEAN
PrtIsEventDeferred(
_In_ PRT_UINT32		eventIndex,
_In_ PRT_UINT32*		defSet
);

FORCEINLINE
PRT_BOOLEAN
PrtIsActionInstalled(
_In_ PRT_UINT32		eventIndex,
_In_ PRT_UINT32*	actionSet
);

FORCEINLINE
PRT_BOOLEAN
PrtIsTransitionPresent(
_In_ PRT_SM_CONTEXT_PRIV	*context,
_In_ PRT_UINT32				eventIndex
);

PRT_BOOLEAN
PrtIsPushTransition(
_In_ PRT_SM_CONTEXT_PRIV		*context,
_In_ PRT_UINT32					event
);

PRT_UINT32 *
PrtClonePackedSet(
_In_ PRT_UINT32 *				packedSet,
_In_ PRT_UINT32					size
);

void
PrtUpdateCurrentActionsSet(
_Inout_ PRT_SM_CONTEXT_PRIV			*context
);

void
PrtUpdateCurrentDeferredSet(
_Inout_ PRT_SM_CONTEXT_PRIV			*context
);

void
PrtResizeEventQueue(
_Inout_ PRT_SM_CONTEXT_PRIV *context
);

void
PrtHaltMachine(
_Inout_ PRT_SM_CONTEXT_PRIV			*context
);

void
PrtCleanupMachine(
_Inout_ PRT_SM_CONTEXT_PRIV			*context
);

void
PrtCleanupModel(
_Inout_ PRT_SM_CONTEXT			*context
);

void
PrtHandleError(
_In_ PRT_STATUS ex,
_In_ PRT_SM_CONTEXT_PRIV *context
);

void
PrtLog(
_In_ PRT_STEP step,
_In_ PRT_SM_CONTEXT_PRIV *context
);

#ifdef __cplusplus
}
#endif
#endif