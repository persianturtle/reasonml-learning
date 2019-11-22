# Session Seven

# [View the recording](https://drive.google.com/open?id=1RK-O_G4VsYvXB0b7bJ0_qbZzSzJJnaUg)

This document serves as a compliment (or an alternative) to the video recordings. Links mentioned in the recordings (as well as additional ones) can be found in their corresponding documents.

## Homework

- [Review previous sessions and/or notes as needed](https://github.com/persianturtle/reasonml-learning/tree/master/sessions)

## Goals

In this session we review the final `<Collapse />` implementation in Reason, and then use the ReasonReact component in ReactJS. Then, we review general functional programming concepts in order to (hopefully) answer common questions.

## Table of Contents

- [Review Learning Session Goals](#review-learning-session-goals) (0:00)
- [Review Collapse Component](#review-collapse-component) (0:59)
- [Demo](#demo) (6:00)
- [What is Functional Programming?](#what-is-functional-programming) (9:53)

## Review Learning Session Goals

- Get a bit familiar with Reason by mapping what we already know to Reason
- Write `<Collapse />` together
- Learn how Reason's type system can help offload invariants from our heads into the codebase
- The after parity path

## Review Collapse Component

Find the code in this repo!

## Demo

We'll see how we can use the `<Collapse />` component in both ReasonML and ReactJS.

## What is Functional Programming?

I was trying to play youtube clips here and had a few issues with the audio in the room we were in. As a result, the video was missing large chunks of audio. I've decided to instead rewrite this content here.

Let's ask the question: Were we "doing" functional programing did during these learning sessions? What even is functional programming?

One definition of functional programming is "programming with functions." Sounds reasonable. To me, the fact that were programming with functions is a side effect of functional programming. Said another way, the _core_ of functional programming goes deeper than that. Let's explore this.

Can you do functional programming with classes and objects?

I understand the core of functional programming to be two concepts:

- [Purity](https://en.wikipedia.org/wiki/Pure_function)
- Immutability ("variables" that never vary)

[YouTube clip on why we care about purity and immutability](https://youtu.be/0if71HOyVjY?t=1059)

[Checking out Elm's performance benefits knowing that it is a pure functional language with only immutable data structures](https://elm-lang.org/)

[Twitter post showing similar performance benefits as it relates to Reason / BuckleScript](https://twitter.com/bobzhang1988/status/827562467148623875)

The TL;DR from the above links is that not only does purity and immutability improve developer experiece, but it _also_ improves user experience (fewer bugs, better performance, etc.).

## Is ReasonML a Functional Language?

If functional programming is all about immutability and purity, and Reason allows mutation and does not enforce purity, is ReasonML a functional language?

[Yaron Minsky clip](https://youtu.be/-J8YyfrSwTk?t=2784)

The TL;DR here is that Reason / OCaml are immutable and pure by _default_, and that is powerful.

## Flavors of FP

There are a lot of functional languages, and each of them have varying support for purity and immutability.

- Reason / OCaml
- F# / Scala
- Haskell
- Elm / PureScript
- Elixir / Erlang
- Closure / ClosureScript
- Lisp / Scheme / Racket

## Recursion

If we're saying that we should be pure and immutable, then how do we do something like this?

```reason
for (i in 1 to 100) {
  Js.log(i)
}
```

[see the compiled JS output and play with the code in the playground](https://reasonml.github.io/try?rrjsx=true&reason=GYewTgBAFAlhMDsIEYIBcQoAxYJQQG8AoCCAKQGcA6AGxAHNZciBfIA)

Is the above pure or impure?

It's impure because we are mutating `i` with each iteration of the loop. (Another reason that it's impure is because of `Js.log`, but let's ignore that for this example.)

So, how can we do this in a pure way (ignoring the `Js.log`)?

Yes! We can use recursion.

```reason
let rec loop = i => {
  Js.log(i)
  if (i < 100) {loop(i + 1)}
}

loop(1);
```

[see the compiled JS output and play with the code in the playground](https://reasonml.github.io/en/try?rrjsx=true&reason=DYUwLgBATiDGHAPaIA4QLwQJYYHwQG8AoCCAKQGcA6JAcwAosBKAbhOwDMJGIAeCAIwAGIU0JJUPANSCmAXyIKiElPQGsiQA)

## Lists

A list is built in data structure in Reason / OCaml. It's a recursive data structure.

A list is either empty or a pair of a value and a list.

To learn more, [read the docs](https://reasonml.github.io/docs/en/list-and-array).

```reason
type list('a) = Empty | Cons('a, list('a));
```

You can write a list like so:

```reason
let list = [1, 2, 3, 4, 5, 6];
```

[see the compiled JS output and play with the code in the playground](https://reasonml.github.io/try?rrjsx=true&reason=DYUwLgBMCWDOkF4IG0CMAaATOgzOgLOgKzoBsAugNxA)

Reason provides some syntactic sugar to represent the `Empty` constructor with `[]` and hides the recursive nature of `Cons`. Without the sugar, we would probably write something like:

```reason
let list =
  Cons(1,
    Cons(2,
      Cons(3,
        Cons(4,
          Cons(5,
            Cons(6, Empty)
          )
        )
      )
    )
  );
```

The compiled output looks like this:

```javascript
// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict'

var list = /* :: */ [
  1,
  /* :: */ [
    2,
    /* :: */ [3, /* :: */ [4, /* :: */ [5, /* :: */ [6, /* [] */ 0]]]]
  ]
]

exports.list = list
/* No side effect */
```

### Sum a list

So let's sum a list.

```reason
let list = [1,2,3,4,5,6];

let rec sum = list => switch(list) {
  | [] => 0
  | [hd, ...tl] => hd + sum(tl)
};

sum(list)->Js.log;
```

[see the compiled JS output and play with the code in the playground](https://reasonml.github.io/try?rrjsx=true&reason=DYUwLgBMCWDOkF4IG0CMAaATOgzOgLOgKzoBsAugNwBQ1okATiAMYSwCuAthEjPDwD42Ad2hhmACwAUfMAEoIAb2oQIAHxTlBEAAwr1KCQBN0EAHQWwwLQiHGIAajZcpVudQC+Nah04y48gC0AgBSsGbAAPYA5pRAA)

This works, but we have an issue. To illustrate the issue, let's break down what's going on here.

First, our list (`[1,2,3,4,5,6]`) is passed in to `sum`.

Let's keep track of the function calls.

```reason
sum([1,2,3,4,5,6])
1 + sum([2,3,4,5,6])
1 + 2 + sum([3,4,5,6])
1 + 2 + 3 + sum([4,5,6])
1 + 2 + 3 + 4 + sum([5,6])
1 + 2 + 3 + 4 + 5 + sum([6])
1 + 2 + 3 + 4 + 5 + 6 + sum([])
1 + 2 + 3 + 4 + 5 + 6 + 0
```

Each line above is a function call. Many languages have the concept of a _stack_.

> The runtime stack
> The run time stack is basically the way your programs store and handle your local non-static variables. Think of the run time stack as a stack of plates. With each function call, a new "plate" is placed onto the stacks. Local variables and parameters are placed onto this plate. Variables from the calling function are no longer accessible (because they are on the plate below). When a function terminates, the local variables and parameters are removed from the stack. Control is then given back to the calling function. Understanding the workings of the run time stack is key to understanding how and why recursion works
> [Source](https://cathyatseneca.gitbooks.io/data-structures-and-algorithms/content/recursion/the_runtime_stack.html)

Each function call is a new "plate" in the stack. We cannot further simplify the stack because we can't simplify `1 + sum([2,3,4,5,6])`. However, if we rewrite our function, we can simplify each "plate" in the stack such that we only ever need one plate for lists of any length.

This process of rewriting our functions such that we only use one "plate" in a stack is called Tail Call Optimization (TCO).

### Tail Call Optimization

Let's rewrite the function above such that it's tail call optimized.

```reason
let list = [1, 2, 3, 4, 5, 6];

let sum = list => {
  let rec aux = (list, acc) =>
    switch (list) {
    | [] => acc
    | [hd, ...tl] => aux(tl, acc + hd)
    };
  aux(list, 0);
};

sum(list)->Js.log;

```

[see the compiled JS output and play with the code in the playground](https://reasonml.github.io/try?rrjsx=true&reason=DYUwLgBMCWDOkF4IG0CMAaCAmTBmTALJgKyYBsAugNwBQNoksArgLYRIzzsB8EA3jQhRwEAE4gAxhACGTAB7sIACk5hM0iRICUPQUIiwA7tDASAFstU6B+iAB8UFHjM16hD5GYAmmAHT+wYCcEXlk5JUD1TQgAaghvLTcIAF9aITCVODUIAAYtWlS6ZhZM+C0AWm4AKVhfYAB7AHNaIA)

Let's now keep track of the function calls.

```reason
sum([1,2,3,4,5,6])
aux([1,2,3,4,5,6], 0)
aux([2,3,4,5,6], 1)
aux([3,4,5,6], 3)
aux([4,5,6], 6)
aux([5,6], 10)
aux([6], 15)
aux([], 21)
21
```

Notice how each "plate" has all the information that is needed and can replace previous plates. _The last thing that our helper function (`aux`) does is call a function_. We're not adding to the result of calling a function. This is all we need to do for TCO.

TCO is an optimization of the _runtime_, not a language. So, does this work? Does JavaScript support TCO? [Well, no.](https://dev.to/snird/recursion-optimization-in-js-where-is-it-ptc-tco-and-fud-4fka)

Have no fear though, Reason compiles this to something that not actually recursive.

```javascript
function sum(list) {
  var _list = list
  var _acc = 0
  while (true) {
    var acc = _acc
    var list$1 = _list
    if (list$1) {
      _acc = (acc + list$1[0]) | 0
      _list = list$1[1]
      continue
    } else {
      return acc
    }
  }
}
```

Note, this is only true if the Reason code is tail call optimized.

### What about immutability? Does that affect performance?

Again, Reason compiles things efficiently, so you don't actually have "copies" in practice. Immutability is enforced at compile time for purposes of code readability, and since the compiler has enough information, it can make nice optimizations.

```reason
let list = [1, 2, 3, 4, 5, 6];
let list2 = [10, ...list];
```

```javascript
var list = /* :: */ [
  1,
  /* :: */ [
    2,
    /* :: */ [3, /* :: */ [4, /* :: */ [5, /* :: */ [6, /* [] */ 0]]]]
  ]
]

var list2 = /* :: */ [10, list]
```

## Why Reason?

At the beginning of these learning sessions, you watched Jared's talk that compared the JavaScript and OCaml ships. Let's [rewatch this](https://youtu.be/RV997YRIMZs) with our new perspective.

The TL;DR here is that OCaml provides a rock solid foundation and Reason adds some of the "fun" from JavaScript. Best of both worlds.

Let's also revisit the [What & Why](https://reasonml.github.io/docs/en/what-and-why) from the docs.

> The language for writing React

The main point here is that Reason is the language for writing React. It's the future of React.

> A few years later, we're now iterating on the future of ReactJS through ReasonReact.

## Where Do Types Fit In?

So if functional programming is all about purity and immutability, then what do types have to do with that? Well, not much, but they are super useful nonetheless.

[Why Types](https://youtu.be/PLFl95c-IiU?t=1426)

[Domain Driven Design Made Functional](https://youtu.be/PLFl95c-IiU?t=1987)

## Thank You For Watching / Reading!

Thank you so much for watching and reading! Please reach out to me on twitter with any questions and comments. Please also submit PRs to correct mistakes!
