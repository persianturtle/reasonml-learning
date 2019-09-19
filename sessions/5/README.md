# Session Five

# [View the recording](https://drive.google.com/open?id=1Ge1waHvlcJUNltXGMnORVVrTams3lqKE)

This document serves as a compliment (or an alternative) to the video recordings. Links mentioned in the recordings (as well as additional ones) can be found in their corresponding documents.

## Homework

- [Review previous sessions and/or notes as needed](https://github.com/persianturtle/reasonml-learning/tree/master/sessions)
- [Review React Hooks](https://reactjs.org/docs/hooks-intro.html) in general with a focus on the [useRef Hook](https://reactjs.org/docs/hooks-reference.html#useref)

## Goals

In this session we're finally starting to rewrite the `<Collapse />` component! This rewrite should span the next two sessions as well. Our goal for now is simply just to translate our existing ReactJS implementation into ReasonReact. After we've gotten the code to a working state, we'll look for opportunities to make things even simpler. As we refactor, we'll see how the compiler can help us!

## Table of Contents

- [Scaffold the Collapse component](#scaffold-the-collapse-component) (0:00)
- [getInitialHeight](#getinitialheight) (6:10)
- [Type Inference](#type-inference) (10:41)
- [componentDidMount Stuff](#componentDidMount-stuff) (12:57)
- [collapsibleRef](#collapsibleref) (13:39)
- [getIsOpen](#getisopen) (14:29)
- [Alias a labelled argument](#alias-a-labelled-argument) (15:13)
- [State](#state) (16:49)
- [More componentDidMount Stuff](#more-componentdidmount-stuff) (21:37)
- [Type System to the Rescue](#type-system-to-the-rescue) (23:09)
- [When Clause](#when-clause) (27:40)
- [Review](#review) (35:20)

## Scaffold the Collapse component

```reason
let defaultDuration = 300;
let defaultTiming = "cubic-bezier(0.09, 1.03, 0.57, 0.97)";

[@react.component]
let make =
  (
    ~isOpen=?,
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
  ) => {};
```

Notice that we're able to default the `~closeDuration` prop to the value of the `~duration` prop!

## getInitialHeight

```reason
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
```

## Type Inference

How does OCaml know the types of the arguments to `getInitialHeight`? Does it rely on what was passed into the function? Watch this segment to find out!

## componentDidMount Stuff

```reason
React.useEffect1(() => None, [||]);
```

This effect will run once and only once since the dependency array is empty. Normal ReactJS stuff. Great, so we'll come back to this effect once we've implemented some other bindings that are used inside of it.

## collapsibleRef

```reason
let collapsibleRef = React.useRef(Js.Nullable.null);
```

Looks familiar from our previous sessions.

## getIsOpen

```reason
let getIsOpen = (~isOpenProp, ~isOpenState) => {
  switch (isOpenProp) {
  | Some(isOpen) => isOpen
  | None => isOpenState
  };
};
```

## Alias a labelled argument

```reason
[@react.component]
let make =
  (
    ~isOpen as isOpenProp=?,
```

## State

```reason
let (isOpenState, setOpen) =
  React.useState(() =>
    switch (isOpenProp) {
    | Some(isOpen) => isOpen
    | None => defaultOpen
    }
  );
```

## More componentDidMount Stuff

```reason
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
```

## Type System to the Rescue

Here, we have an error that I missed, but was given realtime feedback from the typesystem / editor which lead me to catch this much earlier than I otherwise would have.

## When Clause

https://reasonml.github.io/docs/en/pattern-matching#when-clauses

## Review

Here we review switch statements in general as well as what we've done with our `<Collapse />` component so far.

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
