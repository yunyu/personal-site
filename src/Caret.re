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
    IntervalStarted(
      Js.Global.setInterval(() => send(ToggleVisibility), interval),
    )
    |> send,

  willUnmount: ({state}) =>
    switch (state.intervalId) {
    | Some(intervalId) => Js.Global.clearInterval(intervalId)
    | None => ()
    },

  render: ({state}) =>
    <span
      className={
        String.concat(" ", ["caret", state.visible ? "visible" : "hidden"])
      }
    />,
};