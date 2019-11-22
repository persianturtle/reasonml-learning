ReactDOMRe.renderToElementWithId(
  <Uid>
    {uid =>
       <Collapse uid>
         {(
            ~isOpen,
            ~getTogglerProps:
               (~onClick: ReactEvent.Mouse.t => unit=?, unit) =>
               Collapse.togglerProps,
            ~getCollapsibleProps: unit => Collapse.collapsibleProps,
            ~contentRef,
          ) => {
            let togglerProps =
              getTogglerProps(~onClick=_event => Js.log("Huzzah!"), ());
            let collapsibleProps = getCollapsibleProps();
            <>
              <CollapseToggler togglerProps>
                {ReasonReact.string(
                   "Collapsible Content"
                   ++ " - "
                   ++ (isOpen ? "opened" : "closed"),
                 )}
              </CollapseToggler>
              <Collapsible collapsibleProps>
                <div
                  ref={ReactDOMRe.Ref.domRef(contentRef)}
                  style={ReactDOMRe.Style.make(~width="300px", ())}>
                  {ReasonReact.string(
                     "Lorem ipsum dolor sit amet, consectetur adipisicing elit.
                Omnis rem quod error dicta possimus quisquam dolor soluta ea
                modi corporis doloremque reprehenderit fugiat voluptatibus, id
                iure, delectus, laborum voluptas quasi.",
                   )}
                </div>
              </Collapsible>
            </>;
          }}
       </Collapse>}
  </Uid>,
  "root",
);
