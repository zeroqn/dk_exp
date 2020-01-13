function (args) {
  return JSON.stringify({
    ret: adder(args.x + args.y)
  });
};
