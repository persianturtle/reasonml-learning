[@react.component]
let make = () => {
  <CustomId>
    {uid =>
       <p> {ReasonReact.string("the id is: " ++ string_of_int(uid))} </p>}
  </CustomId>;
};
