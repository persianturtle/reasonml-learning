# Session Three

# [View the recording](https://drive.google.com/open?id=13YQOXYnKXIVvI0ZWdGiFEk-tYJI5CYvO)

This document serves as a compliment (or an alternative) to the video recordings. Links mentioned in the recordings (as well as additional ones) can be found in their corresponding documents.

## Homework

- [Review previous sessions and/or notes as needed](https://github.com/persianturtle/reasonml-learning/tree/master/sessions)
- [Review React Hooks](https://reactjs.org/docs/hooks-intro.html) in general with a focus on the [useRef Hook](https://reactjs.org/docs/hooks-reference.html#useref)

## Goals

In this session, we'll learn about using React Refs in Reason. We'll see that React Refs seem to be similar to Reason Refs which we've [previously learned about](https://github.com/persianturtle/reasonml-learning/tree/master/sessions/2#reason-ref).

React Refs will be needed when building the `<Collapse />` component since, for example, we'll need a way to get a DOM element's height in pixels.

## Table of Contents

- [Mutate DOM via useState Hook](#mutate-dom-via-usestate-hook) (0:00)
- [Convert from useState to useRef](#convert-from-usestate-to-useref) (2:50)
- [Reviewing Reason Ref](#reviewing-reason-ref) (15:58)
- [Mutate DOM via useRef](#mutate-dom-via-useref) (19:26)
- [BuckleScript docs](#bucklescript-docs) (36:37)
- [An alternative interop method](#an-alternative-interop-method) (45:23)
- [dangerouslySetInnerHTML](#dangerouslysetinnerhtml)

## Mutate DOM via useState Hook

```reason
[@react.component]
let make = () => {
  let (text, setText) = React.useState(() => "");

  <>
    <button onClick={_event => setText(_text => "Huzzah!")}>
      {ReasonReact.string("Inner Text")}
    </button>
    <input type_="text" value=text />
  </>;
};
```

## Convert from useState to useRef

- remove `useState` stuff
- define a React Ref with an initial string value
- set the `<input />`'s value prop to the current value of the React Ref

```reason
[@react.component]
let make = () => {
  let friendlyGreeting = React.useRef("hi there");

  <>
    <button onClick={_event => ()}>
      {ReasonReact.string("Inner Text")}
    </button>
    <input type_="text" value=React.Ref.current(friendlyGreeting) />
  </>;
};
```

---

- set the current value of the React Ref in `onClick`

```reason
[@react.component]
let make = () => {
  let friendlyGreetingRef = React.useRef("hi there");
  let angryGreeting = "Hi. There.";

  <>
    <button onClick={_event => React.Ref.setCurrent(friendlyGreetingRef, angryGreeting)}>
      {ReasonReact.string("Inner Text")}
    </button>
    <input type_="text" value=React.Ref.current(friendlyGreetingRef) />
  </>;
};
```

_However, clicking on the button here won't cause the `<input />`'s value to change since mutating React Refs don't trigger a re-render!_

**React Refs are instance variables for function components**

## Reviewing Reason Ref

```reason
let foo = ref(5); // ref(...) is a function that initializes a ref

foo := 6; // := sets the current value

Js.log(foo^); // ^ gets the current value
```

---

Again, the above is just syntactic sugar for normal Reason Records.

```reason
type ref('a) = {mutable contents: 'a};

let foo = {contents: 5};

foo.contents = 6;

Js.log(foo.contents);
```

We learned about [Records](https://reasonml.github.io/docs/en/record) last session. We know that they require type definitions. We didn't know that record fields could be [mutable](https://reasonml.github.io/docs/en/mutation)!

## Mutate DOM via useRef

- create a React Ref with an initial value of `null`
- attach that React Ref to the `<input />`'s `ref` prop
- in the `<button />`'s `onClick` prop, we'll mutate `<input />`'s `value` DOM property

```reason
[@react.component]
let make = () => {
  let inputRef = React.useRef(Js.Nullable.null);

  <>
    <button
      onClick={_event =>
        switch (Js.Nullable.toOption(React.Ref.current(inputRef))) {
        | None => ()
        | Some(node) => ReactDOMRe.domElementToObj(node)##value #= "Huzzah!"
        }
      }>
      {ReasonReact.string("Inner Text")}
    </button>
    <input ref={ReactDOMRe.Ref.domRef(inputRef)} type_="text" />
  </>;
};
```

The expression that we `switch` on is current value of the `inputRef` which is converted from a `Js.Nullable.t` top an `option`. In the case where we have a DOM node, we convert the DOM node to a Js object so that we can use the `##` / `#=` syntax to access and mutate properties of that object.

```reason
[@react.component]
let make = () => {
  let inputRef = React.useRef(Js.Nullable.null);

  <>
    <button
      onClick={_event =>
        switch (Js.Nullable.toOption(React.Ref.current(inputRef))) {
        | None => ()
        | Some(ref) =>
          let setValue = [%raw
            {|function(domNode, value) {domNode.value = value; console.log("hmmm");}|}
          ];
          setValue(ref, "Hmm...");
        }
      }>
      {ReasonReact.string("Inner Text")}
    </button>
    <div dangerouslySetInnerHTML={__html: "asdffdaafsd"} />
    <input ref={ReactDOMRe.Ref.domRef(inputRef)} type_="text" />
  </>;
};
```

## BuckleScript docs

Docs for [`##` and `#=` syntax](https://bucklescript.github.io/bucklescript/Manual.html#_binding_to_js_objects)

## An alternative interop method

- Instead of the `##` syntax, we can bind to some existing JavaScript.

```reason
[@react.component]
let make = () => {
  let inputRef = React.useRef(Js.Nullable.null);

  <>
    <button
      onClick={_event =>
        switch (Js.Nullable.toOption(React.Ref.current(inputRef))) {
        | None => ()
        | Some(ref) =>
          let setValue = [%raw
            {|function(domNode, value) {domNode.value = value}|}
          ];
          setValue(ref, "Hmm...");
        }
      }>
      {ReasonReact.string("Inner Text")}
    </button>
    <input ref={ReactDOMRe.Ref.domRef(inputRef)} type_="text" />
  </>;
};
```

## dangerouslySetInnerHTML

Using a `Js.t` object:

```reason
<div dangerouslySetInnerHTML={"__html": "asdf"} />
```

Using `[%raw]`:

```reason
<div dangerouslySetInnerHTML=[%raw {| {__html: "asdf"}|}] />
```
