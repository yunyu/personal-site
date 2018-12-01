let updateArray = (updater, arr) => {
  updater(arr) |> ignore;
  arr;
};

let mapSome = (fn, opt) => Belt.Option.mapWithDefault(opt, (), fn);