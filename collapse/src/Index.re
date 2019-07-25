let s = ReasonReact.string;

ReactDOMRe.renderToElementWithId(
  <>
    <Collapse>
      {(
         ~isOpen,
         ~getTogglerProps:
            (~onClick: ReactEvent.Mouse.t => unit=?, unit) =>
            Collapse.togglerProps,
         ~getCollapsibleProps: Collapse.collapsibleProps,
         ~contentRef,
       ) => {
         let togglerProps =
           getTogglerProps(~onClick=_event => Js.log("Huzzah!"), ());
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
             id={getCollapsibleProps.id}
             ariaHidden={getCollapsibleProps.ariaHidden}
             onTransitionEnd={getCollapsibleProps.onTransitionEnd}
             ref={ReactDOMRe.Ref.domRef(getCollapsibleProps.collapsibleRef)}
             style={ReactDOMRe.Style.make(
               ~height=getCollapsibleProps.style.height,
               ~overflow=getCollapsibleProps.style.overflow,
               ~willChange=getCollapsibleProps.style.willChange,
               ~transitionProperty=
                 getCollapsibleProps.style.transitionProperty,
               ~transitionDuration=
                 getCollapsibleProps.style.transitionDuration,
               ~transitionTimingFunction=
                 getCollapsibleProps.style.transitionTimingFunction,
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
    </Collapse>
  </>,
  "root",
);