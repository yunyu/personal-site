open Belt;
open Util;

type state = {
  displayedChars: array(string),
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

  initialState: () => {displayedChars: [||], timerId: None},

  reducer: (action, state) => {
    let {displayedChars} = state;

    switch (action) {
    | Append(string) =>
      ReasonReact.Update({
        ...state,
        displayedChars: displayedChars->updateArray(Js.Array.push(string)),
      })
    | Backspace =>
      ReasonReact.Update({
        ...state,
        displayedChars: displayedChars->updateArray(Js.Array.pop),
      })
    | TimerStarted(timerId) =>
      ReasonReact.Update({...state, timerId: Some(timerId)})
    };
  },

  didMount: ({send}) => {
    let tasksForString = str => {
      let chars = Js.String.split("", str)->List.fromArray;

      let preTypeTask = [{action: None, nextTaskTimeout: preTypeDelay}];
      let appendTasks =
        chars->List.map(char =>
          {action: Some(Append(char)), nextTaskTimeout: typeDelay}
        );
      let preEraseTask = [{action: None, nextTaskTimeout: preEraseDelay}];
      let eraseTasks =
        chars->List.map(_char =>
          {action: Some(Backspace), nextTaskTimeout: eraseDelay}
        );
      preTypeTask @ appendTasks @ preEraseTask @ eraseTasks;
    };

    let tasks = texts->List.map(tasksForString)->List.flatten;

    let rec runTasks = remaining =>
      switch (remaining) {
      /* Execute current task */
      | [currTask, ...rest] =>
        let {action, nextTaskTimeout} = currTask;
        action->mapSome(send);

        Js.Global.setTimeout(() => runTasks(rest), nextTaskTimeout)
        ->TimerStarted
        ->send;

      /* If no more tasks, restart */
      | [] => runTasks(tasks)
      };

    runTasks(tasks);
  },

  willUnmount: ({state: {timerId}}) =>
    timerId->mapSome(Js.Global.clearTimeout),

  render: ({state}) =>
    <span className="text">
      {Js.Array.joinWith("", state.displayedChars)->ReasonReact.string}
    </span>,
};