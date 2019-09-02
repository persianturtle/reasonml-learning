type ref('a) = {mutable contents: 'a};

let foo = {contents: 5};

foo.contents = 6;