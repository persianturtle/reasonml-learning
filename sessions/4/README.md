# Session Four

# [View the recording](https://drive.google.com/open?id=1ZCA258nY6pTwvm4buYX8tZcDw3u_VP2E)

This document serves as a compliment (or an alternative) to the video recordings. Links mentioned in the recordings (as well as additional ones) can be found in their corresponding documents.

## Homework

- [Review previous sessions and/or notes as needed](https://github.com/persianturtle/reasonml-learning/tree/master/sessions)
- [Review React Hooks](https://reactjs.org/docs/hooks-intro.html) in general with a focus on the [useRef Hook](https://reactjs.org/docs/hooks-reference.html#useref)

## Goals

Since the `<Collapse />` depends on Homebase's `<Uid />` component, we'll use this session to first bind to and then rewrite the `<Uid />` component.

## Table of Contents

- [Bind to Uid](#bind-to-uid) (1:28)
- [Using the bound component](#using-the-bound-component) (6:26)
- [Rewrite Uid](#rewrite-uid) (7:38)
- [useEffect](#useEffect) (16:16)

## Bind to Uid

To bind to an existing ReactJS component, we can create a new Reason file and provide:

- the path to the existing ReactJS component
- the `[@react.component]` decorator
- the type of the `make` function including prop types
- the default or named export of the dependency

`Component.re`

```reason
[@bs.module "./path/to/Component.js"][@react.component]
external make: (~name: string) => React.element = "default";
```

Since `hb-uid.js` exists in `../dependencies/hb-uid.js`, this becomes:

`Hbuid.re`

```reason
[@bs.module "../dependencies/hb-uid.js"] [@react.component]
external make: (~children: int => React.element) => React.element = "default";
```

## Using the bound component

Using the bound component is the same as using a normal ReasonReact component.

`Hbuid.re`

```reason
[@bs.module "../dependencies/hb-uid.js"] [@react.component]
external make: (~children: int => React.element) => React.element = "default";
```

`Demo.re`

```reason
[@react.component]
let make = () => {
  <Hbuid>
    {uid =>
       <p> {ReasonReact.string("the id is: " ++ string_of_int(uid))} </p>}
  </Hbuid>;
};
```

## Rewrite Uid

### Original JS

```javascript
/**
 * HB Unique ID
 *
 * Utility for creating unique IDs
 *
 * @flow
 * @author    Rogin Farrer <rfarrer@wayfair.com>
 * @copyright 2019 Wayfair LLC - All rights reserved
 */
import { Component } from 'react'

export default class Uid extends Component {
  static counter = 0
  state = { uid: 0 }
  componentDidMount() {
    this.setState({ uid: Uid.counter++ })
  }
  render() {
    return this.props.children(this.state.uid)
  }
}
```

### Reason

```reason
let counter = ref(0);

[@react.component]
let make = (~children) => {
  let (uid, setUid) = React.useState(() => 0);

  React.useEffect1(
    () => {
      setUid(_uid => counter^);
      counter := counter^ + 1;
      None;
    },
    [||],
  );

  children(uid);
};
```

Remember, `useEffect` runs on every render by default. From the React Hooks docs:

> If you want to run an effect and clean it up only once (on mount and unmount), you can pass an empty array ([]) as a second argument. This tells React that your effect doesn’t depend on any values from props or state, so it never needs to re-run. This isn’t handled as a special case — it follows directly from how the dependencies array always works.

## useEffect

The type of `React.useEffect` is `unit => option(unit => unit) = unit`. This makes sense once we realize that `useEffect` in ReactJS optionally returns a cleanup function.

The following ReactJS:

```javascript
useEffect(() => {
  console.log('effect')
})
```

corresponds to:

```reason
React.useEffect(() => {
  Js.log("effect");
  None;
})
```

since the last expression of a block is always returned, and the type of `useEffect` requires us to explicitly return `option(unit => unit)`. `None` does meet that requirement.

Alternatively, we could return a cleanup function.

```reason
React.useEffect(() => {
  Js.log("effect");
  Some(() => {Js.log("cleanup on unmount")});
})
```

And ReactJS will call the cleanup function on unmount.

In ReasonReact, we have multiple versions of `React.useEffect`.

- `React.useEffect`
- `React.useEffect0`
- `React.useEffect1`
- `React.useEffect2`
- `React.useEffect3`
- `React.useEffect4`

And the reason for this is that OCaml doesn't allow functions to have an arbitrary number of arguments. Furthermore, arrays in Reason are fix-sized on native (flexibly sized in JavaScript) so the above corresponds to:

- `React.useEffect` => no array argument
- `React.useEffect0` => equivalent to `React.useEffect`
- `React.useEffect1` => empty array as second argument
- `React.useEffect2` => array as second argument with one element
- `React.useEffect3` => array as second argument with two element
- `React.useEffect4` => array as second argument with three element

### ReasonReact on native?

It's interesting to note that the Facebook seems to want ReasonReact to support native platforms since if they didn't, then there would only ever need to be `React.useEffect1` at most since arrays can be flexibly sized in JavaScript. Perhaps one day we'll be able to use ReasonReact directly on native without needing some JavaScript bridge (i.e. React Native).
