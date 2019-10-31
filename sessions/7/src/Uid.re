let counter = ref(0);

[@react.component]
let make = (~children) => {
  let (uid, setUid) = React.useState(() => 0);

  React.useEffect1(
    () => {
      setUid(_uid => counter^);
      counter := counter^ + 1;
      None;
    },
    [||],
  );

  children(uid);
};
