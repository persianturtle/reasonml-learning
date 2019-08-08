# Session Two

# [View the recording](https://drive.google.com/open?id=1PcrpQS0LmqaorIE-1Yy5ykHL3_LmetBV)

This document serves as a compliment (or an alternative) to the video recordings. Links mentioned in the recordings (as well as additional ones) can be found in their corresponding documents.

## Homework

- [Review last week's session video and/or notes as needed](https://github.com/persianturtle/reasonml-learning/tree/master/sessions/1)

## Table of Contents

- [Learning Session Goals](#learning-session-goals) (0:00)
- [Reason Ref](#reason-ref) (0:43)
- [What is a type variable?](#what-is-a-type-variable) (1:03)
- [Type inference](#type-inference) (5:05)
- [Options](#options) (13:13)
- [Labeled arguments](#labeled-arguments) (16:36)
- [Default arguments](#default-arguments) (17:20)
- [Unit](#unit) (18:09)
- [Currying](#currying) (22:29)
- [Optional arguments](#optional-arguments) (30:00)
- [Extending Reason's Type System to the Server](#extending-reasons-type-system-to-the-server) (38:40)
- [Result type](#result-type) (40:10)
- [Variant](#variant) (43:02)

## Learning Session Goals

- Learn about Reason Ref
- Learn about Option
- Learn about Unit
- Use React Ref hook (will be covered in Session Three)

## Reason Ref

We learned about [Records](https://reasonml.github.io/docs/en/record) last session. We know that they require type definitions. We didn't know that record fields could be [mutable](https://reasonml.github.io/docs/en/mutation)!

### Mutable Record

```reason
type ref('a) = {mutable contents: 'a};

let foo = {contents: 5};

foo.contents = 6;

Js.log(foo.contents);
```

However, this idea of a record with a single mutable field is so common that it's provided by the standard library. Its type definition is exactly the same.

```reason
let foo = ref(5);
foo := 6;

Js.log(foo^);
```

The two snippets above comile to the same JavaScript. The latter is "sugar" for the former. The `ref` function along with the `:=` and `^` syntax are also provided by the standard library.

### What is a type variable?

A type variable is a placeholder for a type. In the algebraic expression `y = 2x^2 + x`, if we replace one instance of `x` with the concrete value `5`, then we must replace all instances of `x` with that same value. It's the same for type variables. We must replace type variables with concrete types consistently.

### Type inference

The following is an [excerpt](https://www.cs.cornell.edu/courses/cs3110/2016fa/l/17-inference/notes.html).

OCaml was rather cleverly designed so that type reconstruction is a
straightforward algorithm. At a very high level, that algorithm works as
follows:

- Determine the types of definitions in order, using the types of earlier
  definitions to infer the types of later ones. (Which is one reason you
  may not use a name before it is bound in an OCaml program.)

- For each `let` definition, analyze the definition to determine
  _constraints_ about its type. For example, if the inferencer sees
  `x+1`, it concludes that `x` must have type `int`. It gathers
  similar constraints for function applications, pattern matches, etc.
  Think of these constraints as a system of equations like you might
  have in algebra.

- Use that system of equations to solve for the type of the name
  begin defined.

## Options

[Reason itself doesn't have the notion of null or undefined.](https://reasonml.github.io/docs/en/null-undefined-option)

### Labeled arguments

During the last session, we saw our `add` function.

```reason
let add = (x, y) => x + y;
```

In the above snippet, `x` and `y` are positional arguments meaning that the order of the arguments matter when `add` is called.

```reason
add(4, 5) // x is 4 and y is 5
```

Alternatively, we can use labeled arguments.

```reason
let add = (~x, ~y) => x + y;

add(~y=5, ~x=4) // x is 4 and y is 5
```

### Default arguments

Default arguments can be either positional or labeled.

```reason
let add = (x=0, y) => x + y;
```

### Optional arguments

Similarly, optional arguments can be either positional or labeled.

```reason
let add = (~x=?, ~y) => x + y; // doesn't compile since `x` is now an option

```

### Unit

Unit is exactly nothing. Its type is `unit` and its value is `()`. A function that accepts `unit` cannot also accept a type `int` because `int` is not of type `unit`. Similarly, a function that returns `unit` must only ever return `unit`.

Examples of functions that return `()`:

- click handlers (`onClick: <some event type> => unit`)
- `Js.log` returns `()`

An example of a function that accepts `()`:

- `Js.Math.random` accepts `()` and returns a `float`
- `Js.Date.make` accepts `()` and returns some date type

In the above examples, either accepting or returning `()` is associated with side effects. Of course, we can define a function that doesn't perform a side effect but does return `()`.

```reason
let add = (x, y) => ();
```

But it's unusual to have a function that only accepts `unit` and/or returns `unit` without causing some side effect.

How do you call something that accepts `unit` as a argument? Like this: `Js.Math.random()`.

### Currying

Every function in Reason accepts exactly one argument and returns exactly one expression. If something looks like it accepts no arguments, then it accepts `unit`. Functions that look like they accept two arguments work like the following:

```reason
let add = (x, y) => x + y;
let add = x => y => x + y; // also valid JavaScript
```

And if you partially apply `add`, the resulting expression with be a function waiting for the _next_ argument.

```reason
let addOne = add(1); // y => 1 + y
```

```reason
let five = add(2)(3) // this works in Reason but not in JavaScript
```

The above works in Reason but not in JavaScript because every function is curried in Reason, but that's not the case in JavaScript.

## Extending Reason's Type System to the Server

We'll definitely chat about this in more detail in future sessions.

## Result type

```reason
type result('a, 'b) = Ok('a) | Error('b);
```

The `result` type is quite similar to the `option` type. The tags are named differently but there are still two tags total for each type where `Some` and `Ok` represent the happy path and `None` and `Error` represent the sad path. However, the main difference is that `option` doesn't have data associated with the sad path, but result does. This means that the `result` type is better suited for situations where you want to have data associated with _why_ there was an error.

Of course, you're free to create your own types that express your domain _exactly_. It's just that the `option` and `result` types are so common that they're included in the standard library.

## Variant

- [https://reasonml.github.io/docs/en/variant](https://reasonml.github.io/docs/en/variant)

The `option` and `result` types are both variant types!
