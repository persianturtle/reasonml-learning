# Session Five

# [View the recording](https://drive.google.com/open?id=11AfNXXHE4CdwflcG4YBDlwleq-sb1R1R)

This document serves as a compliment (or an alternative) to the video recordings. Links mentioned in the recordings (as well as additional ones) can be found in their corresponding documents.

## Homework

- [Review previous sessions and/or notes as needed](https://github.com/persianturtle/reasonml-learning/tree/master/sessions)
- [Review React Hooks](https://reactjs.org/docs/hooks-intro.html) in general with a focus on the [useRef Hook](https://reactjs.org/docs/hooks-reference.html#useref)

## Goals

In this session, we continue to rewrite the `<Collapse />` component and get about two-thirds of the way done. After we've gotten the code to a working state, we'll look for opportunities to make things even simpler. As we refactor, we'll see how the compiler can help us!

## Table of Contents

- [Recap](#recap) (0:00)
- [useEffect](#useEffect) (3:33)
- [componentDidUpdate](#componentDidUpdate) (8:27)
- [toggleIsOpen](#toggleIsOpen) (37:42)
- [discussion](#discussion) (40:20)

## Recap

Here's what we have so far.

```reason
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
```

## useEffect

I brought up the point that perhaps ReasonReact is planning on supporting native platforms, but after chatting with Facebook Engineers at ReasonConf, it turns out that the reason for the various versions of useEffect (i.e. useEffect0, useEffect1, useEffect2, etc.) is because Reason's type system can _either_ have a variable number of array elements of the same type, _or_ a set number of array elements of different types.

ReasonReact doesn't yet have any plans for supporting native platforms as far as I know.

## componentDidUpdate

```reason

/* instance variables */
let contentRef = React.useRef(Js.Nullable.null);
let isTransitioning = React.useRef(false);
let transitionTimeoutId = React.useRef(0);
let isClosing = React.useRef(false);

/* ... */

let getTransitionDurationStyle = (~isOpen, ~openDuration, ~closeDuration) => {
  let actualDuration = isOpen ? openDuration : closeDuration;
  string_of_int(actualDuration) ++ "ms";
};

/* ... */

let getHeight = (~isOpen, ~collapsedHeight, ~maxHeight, ~contentRef) => {
  switch (
    isOpen,
    maxHeight,
    React.Ref.current(contentRef)->Js.Nullable.toOption,
  ) {
  | (false, _, _) => string_of_int(collapsedHeight) ++ "px"
  | (_, Some(maxHeight), _) => string_of_int(maxHeight) ++ "px"
  | (_, _, Some(contentRef)) =>
    let contentRef = ReactDOMRe.domElementToObj(contentRef);
    contentRef##clientHeight ++ "px"
  | _ => "auto"
  };
};

/* ... */

/* this corresponds to the JS version's componentDidUpdate */
React.useEffect1(
  () => {
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
        #= getHeight(~isOpen, ~collapsedHeight, ~maxHeight, ~contentRef);
      | None => ()
      };

      requestAnimationFrame(() =>
        requestAnimationFrame(() =>
          if (!React.Ref.current(isClosing)) {
            setClosedStyles();
          }
        )
        ->ignore
      )
      ->ignore;
    };

    None;
  },
  [|isOpen|],
);
```

## toggleIsOpen

Operators like `+`, `-`, `!`, etc. are _functions_ in Reason. When uses these operators as functions, we need to wrap them in `()`.

Then `(+)` is equivalent to the following:

```reason
(a, b) => a + b | 0;
```

and `(!)` is equivalent to the following:

```reason
b => !b
```

So our `toggleIsOpen` function can use `(!)` to toggle the `isOpen` state using the `setOpen` function we defined in the `useState` hook.

```reason
let toggleIsOpen = () => setOpen((!));
```

## Discussion

Here we discuss overall gut reactions.
