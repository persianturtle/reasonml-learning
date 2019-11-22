import React from "react";
import logo from "./logo.svg";
import "./App.css";
const Collapse = require("./Collapse.bs.js").make;
const CollapseToggler = require("./CollapseToggler.bs.js").make;
const Collapsible = require("./Collapsible.bs.js").make;

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.js</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
          style={{ marginBottom: 30 }}
        >
          Learn React
        </a>
        <Collapse>
          {(isOpen, getTogglerProps, getCollapsibleProps, contentRef) => (
            <>
              <CollapseToggler togglerProps={getTogglerProps()}>
                Collapsible Hello {isOpen ? "opened" : "closed"}
              </CollapseToggler>
              <Collapsible collapsibleProps={getCollapsibleProps()}>
                <div ref={contentRef}>Hello from Reason!</div>
              </Collapsible>
            </>
          )}
        </Collapse>
      </header>
    </div>
  );
}

export default App;
