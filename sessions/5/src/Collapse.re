let defaultDuration = 300;
let defaultTimingFunction = "cubic-bezier(0.09, 1.03, 0.57, 0.97)";

let getInitialHeight =
    (~isOpenProp, ~defaultOpen, ~maxHeight, ~collapsedHeight) =>
  switch (isOpenProp, defaultOpen) {
  | (Some(true), _)
  | (_, Some(true)) =>
    switch (maxHeight) {
    | Some(maxHeight) => string_of_int(maxHeight) ++ "px"
    | None => "auto"
    }
  | _ => string_of_int(collapsedHeight) ++ "px"
  };

let getIsOpen = (~isOpenProp, ~isOpenState) =>
  switch (isOpenProp) {
  | Some(isOpen) => isOpen
  | _ => isOpenState
  };

[@react.component]
let make =
    (
      ~isOpen as isOpenProp=?,
      ~defaultOpen=?,
      ~collapsedHeight=0,
      ~duration=defaultDuration,
      ~timingFunction=defaultTimingFunction,
      ~closeDuration=duration,
      ~closeTimingFunction=timingFunction,
      ~openDuration=duration,
      ~openTimingFunction=timingFunction,
      ~maxHeight=?,
      ~children,
    ) => {
  let initialHeight =
    getInitialHeight(~isOpenProp, ~defaultOpen, ~maxHeight, ~collapsedHeight);

  let collapsibleRef = React.useRef(Js.Nullable.null);

  let (isOpenState, setOpen) =
    React.useState(() =>
      switch (isOpenProp, defaultOpen) {
      | (Some(true), _)
      | (_, Some(true)) => true
      | _ => false
      }
    );

  React.useEffect1(
    () => {
      let isOpen = getIsOpen(~isOpenProp, ~isOpenState);
      switch (Js.Nullable.toOption(React.Ref.current(collapsibleRef))) {
      | Some(collapsible) =>
        let collapsible = ReactDOMRe.domElementToObj(collapsible);

        if (isOpen && collapsible##clientHeight != 0) {
          let height =
            switch (maxHeight) {
            | Some(maxHeight) => string_of_int(maxHeight) ++ "px"
            | None => "auto"
            };
          collapsible##style##height #= height;
        };

        if (!isOpen && collapsedHeight == 0) {
          collapsible##style##display #= "none";
        };
      | None => ()
      };

      None;
    },
    [||],
  );

  children(10);
};

/*

 let make =
     (
       ~isOpen=?,
       ~defaultOpen=?,
       ~collapsedHeight=0,
       ~duration=defaultDuration,
       ~timingFunction=defaultTiming,
       ~closeDuration=duration,
       ~closeTimingFunction=timingFunction,
       ~openDuration=duration,
       ~openTimingFunction=timingFunction,
       ~maxHeight=?,
       ~children,
     ) => {

       */
