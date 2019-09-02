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