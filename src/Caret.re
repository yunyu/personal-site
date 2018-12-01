open Util;

type state = {
  visible: bool,
  intervalId: option(Js.Global.intervalId),
};

type action =
  | ToggleVisibility
  | IntervalStarted(Js.Global.intervalId);

let component = ReasonReact.reducerComponent("Caret");

let make = (~interval, _children) => {
  ...component,

  initialState: () => {visible: true, intervalId: None},

  reducer: (action, state) =>
    switch (action) {
    | ToggleVisibility =>
      ReasonReact.Update({...state, visible: !state.visible})
    | IntervalStarted(intervalId) =>
      ReasonReact.Update({...state, intervalId: Some(intervalId)})
    },

  didMount: ({send}) =>
    Js.Global.setInterval(() => send(ToggleVisibility), interval)
    ->IntervalStarted
    ->send,

  willUnmount: ({state: {intervalId}}) =>
    intervalId->mapSome(Js.Global.clearInterval),

  render: ({state: {visible}}) =>
    <span className={Cn.make(["caret", "hidden"->Cn.ifTrue(!visible)])} />,
};