open StackUtil;

type state = {
  displayedChars: Stack.t(string),
  timerId: option(Js.Global.timeoutId),
};

type action =
  | Append(string)
  | Backspace
  | TimerStarted(Js.Global.timeoutId);

type timerTask = {
  action: option(action),
  nextTaskTimeout: int,
};

let component = ReasonReact.reducerComponent("TypedText");

let make =
    (
      ~preTypeDelay,
      ~typeDelay,
      ~preEraseDelay,
      ~eraseDelay,
      ~texts,
      _children,
    ) => {
  ...component,

  initialState: () => {displayedChars: Stack.create(), timerId: None},

  reducer: (action, state) =>
    switch (action) {
    | Append(string) =>
      ReasonReact.Update({
        ...state,
        displayedChars:
          state.displayedChars |> updateStack(Stack.push(string)),
      })
    | Backspace =>
      ReasonReact.Update({
        ...state,
        displayedChars: state.displayedChars |> updateStack(Stack.pop),
      })
    | TimerStarted(timeoutId) =>
      ReasonReact.Update({...state, timerId: Some(timeoutId)})
    },

  didMount: ({send}) => {
    let tasksForString = str => {
      let chars = str |> Js.String.split("") |> Array.to_list;

      let preTypeTask = [{action: None, nextTaskTimeout: preTypeDelay}];
      let appendTasks =
        chars
        |> List.map(char =>
             {action: Some(Append(char)), nextTaskTimeout: typeDelay}
           );
      let preEraseTask = [{action: None, nextTaskTimeout: preEraseDelay}];
      let eraseTasks =
        chars
        |> List.rev_map(_char =>
             {action: Some(Backspace), nextTaskTimeout: eraseDelay}
           );
      preTypeTask @ appendTasks @ preEraseTask @ eraseTasks;
    };

    let tasks = texts |> List.map(tasksForString) |> List.concat;

    let rec runTasks = remaining =>
      switch (remaining) {
      /* Execute current task */
      | [currTask, ...rest] =>
        let {action, nextTaskTimeout} = currTask;

        switch (action) {
        | Some(actionToSend) => send(actionToSend)
        | None => ()
        };

        let timerId =
          Js.Global.setTimeout(() => runTasks(rest), nextTaskTimeout);
        TimerStarted(timerId) |> send;

      /* If no more tasks, restart */
      | [] => runTasks(tasks)
      };

    runTasks(tasks);
  },

  willUnmount: ({state}) =>
    switch (state.timerId) {
    | Some(timerId) => Js.Global.clearTimeout(timerId)
    | None => ()
    },

  render: ({state}) =>
    <span className="text">
      {state.displayedChars |> stringOfStack |> ReasonReact.string}
    </span>,
};