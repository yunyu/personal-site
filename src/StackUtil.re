let updateStack = (updater, stack) => {
  updater(stack);
  stack;
};

let stringOfStack = stack => {
  let out = ref("");
  stack |> Stack.iter(str => out := str ++ out^);
  out^;
};