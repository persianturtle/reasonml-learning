[@react.component]
let make = () => {
  let (count, setCount) = React.useState(() => 0);

  <>
    <p> {React.string("Clicked " ++ string_of_int(count) ++ " times")} </p>
    <button onClick={_event => setCount(count => count + 1)}>
      {React.string("Increase")}
    </button>
    <button onClick={_event => setCount(count => count - 1)}>
      {React.string("Decrease")}
    </button>
  </>;
};