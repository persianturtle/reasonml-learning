[@react.component]
let make = () => {
  let inputRef = React.useRef(Js.Nullable.null);

  <>
    <button
      onClick={_event =>
        switch (Js.Nullable.toOption(React.Ref.current(inputRef))) {
        | None => ()
        | Some(ref) =>
          let setValue: (Dom.element, string) => unit = [%raw
            {|function(domNode, value) {domNode.value = value;}|}
          ];
          setValue(ref, "Hmm...");
        }
      }>
      {ReasonReact.string("Inner Text")}
    </button>
    <input ref={ReactDOMRe.Ref.domRef(inputRef)} type_="text" />
  </>;
};
