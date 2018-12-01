open Belt;

let updateArray = (arr, updater) => {
  updater(arr) -> ignore;
  arr;
};

let mapSome = (opt, fn) => Option.mapWithDefault(opt, (), fn);