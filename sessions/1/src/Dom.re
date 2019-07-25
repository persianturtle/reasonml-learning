%raw
{|
  const button = document.createElement("button");
  const input = document.createElement("input");

  button.innerText = "Input Text";
  input.setAttribute("type", "text");

  button.addEventListener("click", () => {
    input.value = "Huzzah!";
  });

  document.body.appendChild(button);
  document.body.appendChild(input);
|};