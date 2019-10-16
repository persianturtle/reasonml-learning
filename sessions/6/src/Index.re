ReactDOMRe.renderToElementWithId(
  <Collapse>
    {number => <div> {React.string(string_of_int(number))} </div>}
  </Collapse>,
  "root",
);
