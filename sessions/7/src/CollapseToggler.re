[@react.component]
let make = (~togglerProps: Collapse.togglerProps, ~children) => {
  <button
    id={togglerProps.id}
    ariaControls={togglerProps.ariaControls}
    ariaExpanded={togglerProps.ariaExpanded}
    tabIndex={togglerProps.tabIndex}
    type_={togglerProps.type_}
    onClick={togglerProps.onClick}>
    children
  </button>;
};
