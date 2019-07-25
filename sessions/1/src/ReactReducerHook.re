type action =
  | Increment
  | Decrement;

[@react.component]
let make = () => {
  let (count, dispatch) =
    React.useReducer(
      (count, action) =>
        switch (action) {
        | Increment => count + 1
        | Decrement => count - 1
        },
      0,
    );

  <>
    <h1> {ReasonReact.string("Reducer!")} </h1>
    <p> {React.string("Clicked " ++ string_of_int(count) ++ " times")} </p>
    <button onClick={_event => dispatch(Increment)}>
      {React.string("Increase")}
    </button>
    <button onClick={_event => dispatch(Decrement)}>
      {React.string("Decrease")}
    </button>
  </>;
};