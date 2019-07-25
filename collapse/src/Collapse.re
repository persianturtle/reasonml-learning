[@bs.val] external setTimeout: (unit => unit, int) => int = "setTimeout";

type togglerProps = {
  id: string,
  ariaControls: string,
  ariaExpanded: bool,
  tabIndex: int,
  type_: string,
  onClick: ReactEvent.Mouse.t => unit,
};

type collapsibleStyle = {
  height: string,
  overflow: string,
  willChange: string,
  transitionProperty: string,
  transitionDuration: string,
  transitionTimingFunction: string,
};

type collapsibleProps = {
  id: string,
  ariaHidden: bool,
  onTransitionEnd: ReactEvent.Transition.t => unit,
  collapsibleRef: ReactDOMRe.Ref.currentDomRef,
  style: collapsibleStyle,
};

let defaultDuration = 300;
let defaultTiming = "cubic-bezier(0.09, 1.03, 0.57, 0.97)";

let getInitialHeight =
    (~isOpen=?, ~defaultOpen=?, ~maxHeight=?, ~collapsedHeight) => {
  let getHeight =
    switch (maxHeight) {
    | Some(maxHeight) => string_of_int(maxHeight)
    | None => "auto"
    };
  switch (isOpen, defaultOpen) {
  | (Some(true), _) => getHeight
  | (_, Some(true)) => getHeight
  | _ => string_of_int(collapsedHeight) ++ "px"
  };
};

let onTransitionEnd = (_event, ~isOpen, ~setHeight, ~collapsibleRef) =>
  if (isOpen) {
    switch (Js.Nullable.toOption(React.Ref.current(collapsibleRef))) {
    | None => ()
    | Some(node) =>
      if (ReactDOMRe.domElementToObj(node)##clientHeight != 0) {
        setHeight(_height => "auto");
      }
    };
  };

let getTogglerProps = (~onClick=?, ~isOpen, ~setOpen, ~counter, ()) => {
  id: "CollapseToggle-" ++ string_of_int(counter),
  ariaControls: "CollapsePanel-" ++ string_of_int(counter),
  ariaExpanded: isOpen,
  tabIndex: 0,
  type_: "button",
  onClick: event => {
    switch (onClick) {
    | Some(onClick) => onClick(event)
    | None => ()
    };
    setOpen((!));
  },
};

let getTransitionDurationStyle = (~isOpen, ~openDuration, ~closeDuration) => {
  let actualDuration = isOpen ? openDuration : closeDuration;
  string_of_int(actualDuration) ++ "ms";
};

let getTransitionTimingFunctionStyle =
    (~isOpen, ~openTimingFunction, ~closeTimingFunction) => {
  isOpen ? openTimingFunction : closeTimingFunction;
};

let getCollapsibleProps =
    (
      ~isOpen,
      ~counter,
      ~openDuration,
      ~closeDuration,
      ~openTimingFunction,
      ~closeTimingFunction,
      ~height,
      ~setHeight,
      ~collapsibleRef,
    ) => {
  id: "CollapsePanel-" ++ string_of_int(counter),
  ariaHidden: !isOpen,
  onTransitionEnd: onTransitionEnd(~isOpen, ~setHeight, ~collapsibleRef),
  collapsibleRef,
  style: {
    height,
    overflow: "hidden",
    willChange: "height",
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

let getHeight = (~maxHeight, ~contentRef) =>
  switch (maxHeight, Js.Nullable.toOption(React.Ref.current(contentRef))) {
  | (Some(maxHeight), _) => string_of_int(maxHeight) ++ "px"
  | (_, Some(node)) =>
    ReactDOMRe.domElementToObj(node)##clientHeight->string_of_int ++ "px"
  | _ => "auto"
  };

[@react.component]
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
  let (height, setHeight) =
    React.useState(() =>
      getInitialHeight(~isOpen?, ~defaultOpen?, ~maxHeight?, ~collapsedHeight)
    );

  let (isOpen, setOpen) =
    React.useState(() =>
      switch (isOpen, defaultOpen) {
      | (Some(true), _) => true
      | (_, Some(true)) => true
      | _ => false
      }
    );

  let (counter, setCounter) = React.useState(() => 0);

  let isClosing = React.useRef(false);
  let contentRef = React.useRef(Js.Nullable.null);
  let collapsibleRef = React.useRef(Js.Nullable.null);

  React.useEffect1(
    () => {
      setCounter(counter => counter + 1);
      None;
    },
    [||],
  );

  React.useLayoutEffect1(
    () => {
      if (isOpen) {
        isClosing->React.Ref.setCurrent(false);
        setHeight(_height => getHeight(~maxHeight, ~contentRef));
      } else {
        isClosing->React.Ref.setCurrent(true);
        setHeight(_height => getHeight(~maxHeight, ~contentRef));
      };

      None;
    },
    [|isOpen|],
  );

  React.useLayoutEffect1(
    () => {
      setTimeout(
        () =>
          if (isClosing->React.Ref.current) {
            setHeight(_height => "0px");
          },
        0,
      )
      ->ignore;
      None;
    },
    [|isOpen|],
  );

  children(
    ~isOpen,
    ~getTogglerProps=getTogglerProps(~isOpen, ~setOpen, ~counter),
    ~getCollapsibleProps=
      getCollapsibleProps(
        ~isOpen,
        ~counter,
        ~openDuration,
        ~closeDuration,
        ~openTimingFunction,
        ~closeTimingFunction,
        ~height,
        ~setHeight,
        ~collapsibleRef,
      ),
    ~contentRef,
  );
};