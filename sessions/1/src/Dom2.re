type document;
type element;
[@bs.val] external document: document = "document";
[@bs.val] [@bs.scope "document"] external body: element = "body";
[@bs.send]
external createElement: (document, string) => element = "createElement";
[@bs.set] external innerText: (element, string) => unit = "innerText";
[@bs.set] external value: (element, string) => unit = "value";
[@bs.send]
external setAttribute: (element, string, string) => unit = "setAttribute";
[@bs.send]
external addEventListener: (element, string, unit => unit) => unit =
  "addEventListener";
[@bs.send] external appendChild: (element, element) => unit = "appendChild";

let button = createElement(document, "button");
let input = createElement(document, "input");

innerText(button, "Inner Text");
setAttribute(input, "type", "text");

addEventListener(button, "click", () => value(input, "Huzzah!"));

appendChild(body, button);
appendChild(body, input);