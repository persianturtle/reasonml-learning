[@bs.val] external clearTimeout: int => unit = "clearTimeout";
[@bs.val] external setTimeout: (unit => unit, int) => int = "setTimeout";
[@bs.val]
external requestAnimationFrame: (unit => unit) => int =
  "requestAnimationFrame";

type togglerProps = {
  id: string,
  ariaControls: string,
  ariaExpanded: bool,
  tabIndex: int,
  type_: string,
  onClick: ReactEvent.Mouse.t => unit,
};

type style = {
  height: string,
  overflow: string,
  transitionProperty: string,
  transitionDuration: string,
  transitionTimingFunction: string,
};

type collapsibleProps = {
  id: string,
  ariaHidden: bool,
  onTransitionEnd: ReactEvent.Transition.t => unit,
  ref: React.Ref.t(Js.Nullable.t(Dom.element)),
  style,
};

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

let getTransitionDurationStyle = (~isOpen, ~openDuration, ~closeDuration) => {
  let actualDuration = isOpen ? openDuration : closeDuration;
  string_of_int(actualDuration) ++ "ms";
};

let getTransitionTimingFunctionStyle =
    (~isOpen, ~openTimingFunction, ~closeTimingFunction) => {
  isOpen ? openTimingFunction : closeTimingFunction;
};

let getHeight = (~isOpen, ~collapsedHeight, ~maxHeight, ~contentRef) => {
  switch (
    isOpen,
    maxHeight,
    React.Ref.current(contentRef)->Js.Nullable.toOption,
  ) {
  | (false, _, _) => string_of_int(collapsedHeight) ++ "px"
  | (_, Some(maxHeight), _) => string_of_int(maxHeight) ++ "px"
  | (_, _, Some(content)) =>
    let content = ReactDOMRe.domElementToObj(content);
    content##clientHeight ++ "px";
  | _ => "auto"
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
      ~uid,
      ~children,
    ) => {
  let collapsibleRef = React.useRef(Js.Nullable.null);
  let contentRef = React.useRef(Js.Nullable.null);
  let isTransitioning = React.useRef(false);
  let transitionTimeoutId = React.useRef(0);
  let isClosing = React.useRef(false);

  let (isOpenState, setOpen) =
    React.useState(() =>
      switch (isOpenProp) {
      | Some(isOpen) => isOpen
      | None => defaultOpen
      }
    );

  let isOpen = getIsOpen(~isOpenProp, ~isOpenState);

  let setOpenedStyles = () => {
    switch (React.Ref.current(collapsibleRef)->Js.Nullable.toOption) {
    | Some(collapsible) =>
      let collapsible = ReactDOMRe.domElementToObj(collapsible);
      collapsible##style##willChange #= "height";
      collapsible##style##display #= "";

      requestAnimationFrame(() =>
        if (!React.Ref.current(isClosing)) {
          collapsible##style##height
          #= getHeight(
               ~isOpen=true,
               ~collapsedHeight,
               ~maxHeight,
               ~contentRef,
             );
        }
      )
      ->ignore;
    | None => ()
    };
  };

  let setClosedStyles = () => {
    switch (React.Ref.current(collapsibleRef)->Js.Nullable.toOption) {
    | Some(collapsible) =>
      let collapsible = ReactDOMRe.domElementToObj(collapsible);
      collapsible##style##willChange #= "height";

      requestAnimationFrame(() =>
        if (React.Ref.current(isClosing)) {
          collapsible##style##height
          #= getHeight(
               ~isOpen=false,
               ~collapsedHeight,
               ~maxHeight,
               ~contentRef,
             );
        }
      )
      ->ignore;
    | None => ()
    };
  };

  let toggleIsOpen = () => setOpen((!));

  let rec handleCollapsibleTransitionEnd = event => {
    switch (
      React.Ref.current(isTransitioning),
      React.Ref.current(collapsibleRef)->Js.Nullable.toOption,
    ) {
    | (true, _) =>
      requestAnimationFrame(() => handleCollapsibleTransitionEnd(event))
      ->ignore
    | (false, Some(collapsible)) =>
      let collapsible = ReactDOMRe.domElementToObj(collapsible);
      if (isOpen) {
        collapsible##style##willChange #= "";

        if (collapsible##style##clientHeight != 0) {
          collapsible##style##height
          #= {
               switch (maxHeight) {
               | Some(maxHeight) => string_of_int(maxHeight) ++ "px"
               | None => "auto"
               };
             };
        };
      } else if (collapsedHeight == 0) {
        collapsible##style##display #= "none";
      };
    | (false, None) => Js.log("collapsible ref is null")
    };
  };

  let getTogglerProps = (~onClick=?, ()) => {
    id: "CollapseToggle-" ++ string_of_int(uid),
    ariaControls: "CollapsePanel-" ++ string_of_int(uid),
    ariaExpanded: isOpen,
    tabIndex: 0,
    type_: "button",
    onClick: event => {
      switch (onClick) {
      | Some(onClick) => onClick(event)
      | None => ()
      };
      toggleIsOpen();
    },
  };

  let getCollapsibleProps = () => {
    id: "CollapsePanel-" ++ string_of_int(uid),
    ariaHidden: !isOpen,
    onTransitionEnd: handleCollapsibleTransitionEnd,
    ref: collapsibleRef,
    style: {
      height:
        switch (React.Ref.current(collapsibleRef)->Js.Nullable.toOption) {
        | Some(collapsible) =>
          let collapsible = ReactDOMRe.domElementToObj(collapsible);
          collapsible##style##height;
        | None =>
          getInitialHeight(
            ~isOpen=isOpenProp,
            ~defaultOpen,
            ~maxHeight,
            ~collapsedHeight,
          )
        },
      overflow: "hidden",
      transitionProperty: "height",
      transitionDuration:
        getTransitionDurationStyle(~isOpen, ~openDuration, ~closeDuration),
      transitionTimingFunction:
        getTransitionTimingFunctionStyle(
          ~isOpen,
          ~openTimingFunction,
          ~closeTimingFunction,
        ),
    },
  };

  React.useEffect1(
    () => {
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

  Js.log(React.Ref.current(isTransitioning));

  React.useEffect(() => {
    React.Ref.setCurrent(isTransitioning, true);
    clearTimeout(React.Ref.current(transitionTimeoutId));

    React.Ref.setCurrent(
      transitionTimeoutId,
      {
        setTimeout(
          () => React.Ref.setCurrent(isTransitioning, false),
          getTransitionDurationStyle(~isOpen, ~openDuration, ~closeDuration)
          ->Belt.Int.fromString
          ->Belt.Option.getWithDefault(duration),
        );
      },
    );

    if (isOpen) {
      React.Ref.setCurrent(isClosing, false);
      setOpenedStyles();
    } else {
      React.Ref.setCurrent(isClosing, true);

      switch (React.Ref.current(collapsibleRef)->Js.Nullable.toOption) {
      | Some(collapsible) =>
        let collapsible = ReactDOMRe.domElementToObj(collapsible);
        collapsible##style##height
        #= getHeight(~isOpen=true, ~collapsedHeight, ~maxHeight, ~contentRef);
      | None => ()
      };

      requestAnimationFrame(() =>
        requestAnimationFrame(() =>
          if (React.Ref.current(isClosing)) {
            setClosedStyles();
          }
        )
        ->ignore
      )
      ->ignore;
    };

    None;
  });

  children(~isOpen, ~getTogglerProps, ~getCollapsibleProps, ~contentRef);
};
