A Function in P can be arbitrary piece of imperative code which enables programmers to capture complex protocol logic in their state machines.
P supports common imperative programming language statement constructs like while-loops, function calls, and conditionals.

??? Note "P Statements Grammar"
    ```
    statement : { statement* }							# CompoundStmt
    | assert expr (, expr)? ;	                        # AssertStmt
    | print expr ;						                # PrintStmt
    | while ( expr )  statement			                # WhileStmt
    | if ( expr ) statement (else statement)?           # IfThenElseStmt
    | break ;											# BreakStmt
    | continue ;										# ContinueStmt
    | return expr? ;									# ReturnStmt
    | lvalue = rvalue ;							        # AssignStmt
    | lvalue += ( expr, rvalue ) ;	                    # InsertStmt
    | lvalue += ( rvalue ) ;				            # AddStmt
    | lvalue -= rvalue ;							    # RemoveStmt
    | new iden (rvalue?) ;				                # CtorStmt
    | iden ( rvalueList? ) ;				            # FunCallStmt
    | raise expr (, rvalue)? ;			                # RaiseStmt
    | send expr, expr (, rvalue)? ;	                    # SendStmt
    | annouce expr (, rvalue)? ;				        # AnnounceStmt
    | goto iden (, rvalue)? ;				            # GotoStmt
    | receive { recvCase+ }						        # ReceiveStmt
    ;

    /** l-value may appear as left hand of an assignment operator(=) **/
    lvalue : name=iden        # VarLvalue
    | lvalue . field=iden     # NamedTupleFieldAccess
    | lvalue . int            # TupleFieldAccess
    | lvalue [ expr ]         # CollectionLookUp
    ;

    /* A r-value is an expression that can’t have a value assigned to it which 
    means r-value can appear on right but not on left hand side of an assignment operator(=)*/
    rvalue : expr ;
    # rvalueList is a comma separated list of rvalue.
    
    /* case block inside a receive statement */
    recvCase : case eventList : anonFunction
    ```
    
    The `expr` is any expression in P defined by the grammar desbribed in [P Expressions](expressions.md) 

### Assert

P allows writing local assertions using the  `assert` statement. 
If the program violates the assertion then a counter example is generated by the P checker.

**Syntax**: ```assert expr (, expr)? ;```

The assert statement must have a `boolean` expression followed by an optional `string` message that is printed in the error trace. 

=== "Assert"

    ``` kotlin 
    assert (requestId > 1) && (requestId in allRequestsSet);
    ```

    Assert that the `requestId` is always greater than `1` and is in the set of all requests.

=== "Assert with Error Message"

    ``` kotlin
    assert x >= 0, "Expected x to be always positive";
    ```

    The assert statement can have a `string` message which is printed in the error trace.

=== "Assert with Formatted Error Message"

    ``` kotlin 
    assert (requestId in allRequestsSet),
    format ("requestId {0} is not in the requests set = {1}", requestId, allRequestsSet);
    ```

    Assert that the `requestId` is in the set of all requests. 
    You can also provide a formatted string error message to add details to the error message generated.

### Print

Print statements can be used for writing or printing log messages into the error traces (especially for debugging purposes).

**Syntax**: ```print expr ;```

The print statement must have an expression of type `string`.

=== "Print Simple Message"

    ``` kotlin
    print "Hello World!";
    ```

    Print "Hello World!" in the execution trace log.

=== "Print Formatted String Message"

    ``` kotlin
    x = "You";
    print format("Hello World to {0}!!", x);
    ```

    Print "Hello World to You!!" in the execution trace log.

### While

While statement in P is just like while loops in other popular programming languages like C, C# or Java.

**Syntax**: ```while ( expr )  statement```

`expr` is the conditional `boolean` expression and `statement` could be any P statement.

=== "While Loop"

    ``` kotlin
    i = 0;
    while (i < 10)
    { 
        ...
        i = i + 1;
    }  
    ```

=== "While loop iterating over collection"

    ``` kotlin
    i = 0;
    while (i < sizeof(s))
    { 
        ...
        print s[i];
        i = i + 1;
    }
    ```

### If Then Else

IfThenElse statement in P is just like conditional if statements in other popular programming languages like C, C# or Java.

**Syntax**: ```if ( expr ) statement (else statement)?```

`expr` is the conditional `boolean` expression and `statement` could be any P statement. The `else` block is optional.

=== "If Statement"

    ``` kotlin
    if(x > 10) { 
        ...
        x = x + 20; 
    }  
    ```

=== "If Else Statement"

    ``` kotlin
    if(x > 10) 
    { 
        x = 0; 
    }
    else
    { 
        x = x + 1; 
    }
    ```

### Break and Continue

`break` and `continue` statements in P are just like in other popular programming languages like C, C# or Java 
to break out of the while loop or to continue to the next iteration of the loop respectively.

=== "Break"

    ``` kotlin
    while(true) {
        ...
        if(x == 10)
            break;
        ...
        x = x + 1;
    }  
    ```

=== "Continue"

    ``` kotlin
    while(true) {
        ...
        if(x == 10) // skip the loop when x is 10
            continue;
        ...
    }
    ```

### Return

`return` statement in P can be used to return (or return a value) from any function.

=== "Return"

    ``` kotlin
    fun IncrementX() {
        if(x > MAX_INT)
            return;
        x = x + 1;
    }
    ```

=== "Return Value"

    ``` kotlin
    fun Max(x: int, y: int) : int{
    if(x > y)
        return x;
    else
        return y;
    }
    ```

### Assignment

!!! warning "Value Semantics"
    Recollect that P has **value semantics** or **copy-by-value semantics** and does not support any notion of references.

**Syntax**: `lvalue = rvalue ;`

Note that because of value semantics assignment in P copies the value of the `rvalue` into `lvalue`.
`lvalue` could be any variable, a tuple field access, or an element in a collection as described in the Grammar above.
`rvalue` could be any expression that evaluates to the same type as `lvalue`.

=== "Assignment is copying!"

    ``` kotlin
    var a: seq[string];
    var b: seq[string];
    b += (0, "b");
    a = b; // copy value
    a += (1, "a");
    print a; // will print ["b", "a"]
    print b; // will print ["b"]
    ```

=== "Assignments .."

    ``` kotlin
    a = 10; s[i] = 20; tup1.a = "x";
    tup2.0 = 10; t = foo();
    ```

### Insert

Insert statement is used to insert or add an element into a collection.

**Syntax**: `lvalue += ( expr, rvalue ) ;` or `lvalue += ( rvalue ) ;`

`lvalue` is a value of any collection type in P. 

=== "Insert into a Sequence"

    ``` kotlin
    var sq : seq[T];
    var x: T, i : int;

    // add x into the sequence sq at index i
    sq += (i, x);
    ```

    !!! warning "Index for a sequence"
        The value of index `i` above should be between `0 <= i <= sizeof(sq)`. 
        `i = 0` insserts x at the start of `sq` and `i = sizeof(sq)` appends x at the end of `sq`

=== "Insert into a map or update map"

    ``` kotlin
    var mp : map[K,V];
    var x: K, y: V;

    // adds (x, y) into the map
    mp += (x, y);
    // adds (x, y) into the map, if key x already exists then updates its value to y.
    mp[x] = y;
    ```

=== "Insert or add into a set"

    ``` kotlin
    var st : set[T];
    var x: T;

    // adds x into the set st
    st += (x);
    ```

### Remove

Remove statement is used to remove an element from a collection.

**Syntax**: `lvalue -= rvalue ;`

=== "Remove from a Sequence"

    ``` kotlin
    var sq : seq[T];
    var i : int;

    // remove element at index i in the sequence sq
    sq -= (i);
    ```

    !!! warning "Index for a sequence"
        The value of index `i` above should be between `0 <= i <= sizeof(sq) - 1`.

=== "Remove from a map"

    ``` kotlin
    var mp : map[K,V];
    var x: K;

    // Removes the element (x, _) from the map i.e., removes the element with key x from mp
    mp -= (x);
    ```

=== "Remove from a set"

    ``` kotlin
    var st : set[T];
    var x: T;

    // removes x from the set st
    st -= (x);
    ```

### New

New statement is used to create an instance of a machine in P.

**Syntax**: `new iden (rvalue?) ;`

=== "Create a machine"

    ``` java
    new Client((id = 1, server = this));
    ```
    Creates a dynamic instance of a Client machine and passes the constructor parameter `(id = 1, server = this)` 
    which is delivered as a payload to the entry function of the start state of the created Client machine.

### Function Call

Function calls in P are similar to any other imperative programming languages.

!!! Note ""
    Note that the parameters passed to the functions and the return values are pass-by-value!

**Syntax**: `iden ( rvalue? ) ;`

=== "Function call"

    ``` java
    Foo(); Bar(10, "haha");
    ```

### Raise

The statement `raise e, v;` terminates the evaluation of the function raising an event `e` with payload `v`. The
control of the state machine jumps to end of the entry function (popping the function stack if raise is trigger inside a nested function),
and the state machine immediately handles the raised event. One can think of raise of an event as throwing
an exception which terminates the execution of the function stack and must be immediately handled by the event-handlers defined in that state.

**Syntax**: `raise expr (, rvalue)?`

=== "Raise Event"

    ``` java
    state HandleRequest {
        entry (req: tRequest)
        {
            // ohh, this is a Add request and I have a event handler for it 
            if(req.type == "Add")
                raise eAddOperation, req.Transaction; // terminates function
            .....
            .....
            assert req.type != "Add"; // valid
        }
        
        on eAddOperation do (trans: tTransaction) { ... }
    }
    ```

=== "Non deterministically triggering event handlers internally"

    ``` java
        state DoAddOrRemove {
            entry
            {
                /* I am uncertain, at this point I may want to trigger 
                a Add or Substract event-handler without sending an event 
                to self which will be enqueued and then dequeued in FIFO order
                I want to immediately execute this handlers before anything else */
                if($)
                    raise eAddOperation, transaction; // terminates function
                else
                    raise eRemoveOperation, transaction; // terminates function

                assert false; // valid, as this is unreachable
            }
            
            on eAddOperation do (trans: tTransaction) { ... }
            on eRemoveOperation do (trans: tTransaction) { ... }
        }
    ```

### Send

Send statement is one of the most important statements in P as it is used to send messages to other state machines :innocent:.
Send takes as argument a triple `send t, e, v`, where `t` is a reference to the target state machine, `e` is the event sent and `v` is the associated payload.

**Syntax**: `send expr, expr (, rvalue)? ;`

Sends in P are asynchronous and non-blocking. Statement `send t, e, v` enqueues the event `e` with payload `v` into the target machine `t`'s message buffer.

=== "Send event with payload"

    ``` kotlin
    send server, eRequest, (source = this, reqId = 0);
    ```

=== "Send event"

    ``` kotlin
    send server, ePing;
    ```

### Announce

Announce is used to publish messages to specification monitors in P.
When writing specifications there are instances when we would like to send additional information to monitors that is not
captured in the events exchanged between state machines.
Recollect that [spec monitors](monitors.md) in P follow a publish-subscribe model of communication.
Each monitor `observes` a set of events and whenever a machine sends an event that is in the `observes` set of a monitor
then it is synchronously delivered to the monitor.
Announce can be used to publish an event to all the monitors that are observing that event.
The [Two phase commit](../tutorial/twophasecommit.md) example provides an use case for announce.

**Syntax**: `annouce expr (, rvalue)? ;`

=== "Announce event"
  
    ``` kotlin
    spec CheckConvergedState observes eStateUpdate, eSystemConverged 
    { ... }
    ```
    Consider a specification monitor that continuously observes `eStateUpdate` event to keep track of the system state
    and then asserts the required property when the system converges.
    We can use an announce statement to inform the monitor when the system has converged and we should to assert the global specification.
    ```

    announce eSystemConverged, payload; 
    ```

!!! note
    Announce only delivers events to specification monitors (not state machines) and hence has no side effect on the system behavior.
    Announce is used for passing information to the monitors during system execution which the monitors can use to assert global specifications about the system.

### Goto

Goto statement can be used to jump to a particular state. On executing a goto, the state machine exits the current state
(terminating the execution of the current function) and enters the target state.
The optional payload accompanying the goto statement becomes the input parameter to the entry function of the target state.

**Syntax**: `goto iden (, rvalue)? ;`

=== "Goto"
    ``` java
    state ServicePendingRequests {
        entry {
            if(sizeof(pendingRequests) == 0)
                goto Done;

            // process requests 
            ....
        }
    }

    state Done { ... }
    ```
=== "Goto with payload"
    ``` java
    state ServiceRequests {
        entry (req: tRequest) {
            // process request with some complicated logic
            ...
            lastReqId = req.Id;
            goto WaitForRequests, lastReqId;
        }
    }

    state WaitForRequests {
        entry (lastReqId: int) { ... }
    }
    ```

### Receive

Receive statements in P are used to perform blocking await/receive for a set of events inside a function.

**Syntax**:

    ```
    receive { recvCase+ }
    /* case block inside a receive statement */
    recvCase : case eventList : anonFunction
    ```

Each `receive` statement can block or wait on a set of events, all other events are automatically deferred by the state machine. 
On receiving an event that the `receive` is blocking on (case blocks), the state machine unblocks, executes the corresponding case-handler 
and resumes executing the next statement after receive.

=== "Receive: await single event"

    ``` java
    fun AcquireLock(lock: machine)
    {
        send lock, eAcquireLock;
        receive {
            case eLockGranted: (result: tResponse) { /* case handler */ }
        }
        print "Lock Acquired!"
    }
    ```
    Note that when executing the AcquireLock function the state machine blocks at the receive statement, 
    it automatically defers all the events except the `eLockGranted` event. On receiving the `eLockGranted`, 
    the case-handler is executed and then the print statement.

=== "Receive: await multiple events"

    ``` java
    fun WaitForTime(timer: Timer, time: int)
    {
        var success: bool;
        send timer, eStartTimer, time;
        receive {
            case eTimeOut: { success = true; }
            case eStartTimerFailed: { success = false; }
        }
        if (success) print "Successfully waited!"
    }
    ```
    Note that when executing the WaitForTime function the state machine blocks at the receive statement,
    it automatically defers all the events except the `eTimeOut` and `eStartTimerFailed` events.