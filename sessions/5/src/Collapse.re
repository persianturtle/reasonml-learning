let defaultDuration = 300;
let defaultTiming = "cubic-bezier(0.09, 1.03, 0.57, 0.97)";

let getInitialHeight = (~isOpen, ~defaultOpen, ~maxHeight, ~collapsedHeight) => {
  switch (isOpen, defaultOpen) {
  | (Some(true), _)
  | (_, true) =>
    switch (maxHeight) {
    | Some(maxHeight) => string_of_int(maxHeight) ++ "px"
    | None => "auto"
    }
  | _ => string_of_int(collapsedHeight) ++ "px"
  };
};

let getIsOpen = (~isOpenProp, ~isOpenState) => {
  switch (isOpenProp) {
  | Some(isOpen) => isOpen
  | None => isOpenState
  };
};

[@react.component]
let make =
    (
      ~isOpen as isOpenProp=?,
      ~defaultOpen=false,
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
  let collapsibleRef = React.useRef(Js.Nullable.null);

  let (isOpenState, setOpen) =
    React.useState(() =>
      switch (isOpenProp) {
      | Some(isOpen) => isOpen
      | None => defaultOpen
      }
    );

  React.useEffect1(
    () => {
      let isOpen = getIsOpen(~isOpenProp, ~isOpenState);
      switch (React.Ref.current(collapsibleRef)->Js.Nullable.toOption) {
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
};
