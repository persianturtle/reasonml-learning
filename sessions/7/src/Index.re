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
              <button
                id={togglerProps.id}
                ariaControls={togglerProps.ariaControls}
                ariaExpanded={togglerProps.ariaExpanded}
                tabIndex={togglerProps.tabIndex}
                type_={togglerProps.type_}
                onClick={togglerProps.onClick}>
                {ReasonReact.string(
                   "Collapsible Content"
                   ++ " - "
                   ++ (isOpen ? "opened" : "closed"),
                 )}
              </button>
              <div
                id={collapsibleProps.id}
                ariaHidden={collapsibleProps.ariaHidden}
                onTransitionEnd={collapsibleProps.onTransitionEnd}
                ref={ReactDOMRe.Ref.domRef(collapsibleProps.ref)}
                style={ReactDOMRe.Style.make(
                  ~height=collapsibleProps.style.height,
                  ~overflow=collapsibleProps.style.overflow,
                  ~transitionProperty=
                    collapsibleProps.style.transitionProperty,
                  ~transitionDuration=
                    collapsibleProps.style.transitionDuration,
                  ~transitionTimingFunction=
                    collapsibleProps.style.transitionTimingFunction,
                  (),
                )}>
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
              </div>
            </>;
          }}
       </Collapse>}
  </Uid>,
  "root",
);
