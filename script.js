let wasm;
let canvas;
let ctx;
let previous;
let dt;
let blured = false;
const str_len = wasmlib.str_len;
const get_str = wasmlib.get_str;
let ddrawRectangle;
WebAssembly.instantiateStreaming(fetch("game.wasm"), {
  env: wasmlib.make_environment({
    jprintf: wasmlib.printf,
    // abs: (n) => {
    //   return Math.abs(n);
    // },
    // sin: (n) => {
    //   return Math.sin(n);
    // },
    // cos: (n) => {
    //   return Math.cos(n);
    // },
    // tanh: (n) => {
    //   return Math.tanh(n);
    // },
    // exp: (n) => {
    //   return Math.exp(n);
    // },
  }),
}).then((w) => {
  wasm = w;
  canvas = document.getElementById("canvas");
  ctx = canvas.getContext("2d");
  const {
    setBackgroundColor,
    change_buffer,
    drawRectangle,
    getBuffer,
    draw_shader,
    get_image_size,
    get_image_width,
    get_image_height,
  } = w.instance.exports;
  const buffer = wasm.instance.exports.memory.buffer;
  let bg_color = 0x6b5b95ff;
  //0xff955b6b
  let width = get_image_width();
  let height = get_image_height();
  canvas.width = width;
  canvas.height = height;
  let buff = new Uint8ClampedArray(
    buffer,
    setBackgroundColor(0x6b5b95ff), //rgba
    width * height * 4
  );
  // console.log(setBackgroundColor(0x6b5b95ff));
  console.log(new Uint8ClampedArray(buffer, getBuffer(), width * height * 4));
  ddrawRectangle = drawRectangle;
  // drawRectangle(300, 400, 400, 200, 0xffff00ff);
  const image = new ImageData(buff, width, height);
  // change_buffer(buffer_ptr());
  let previous;
  let i = 0;
  canvas.style.transformOrigin = "0 0";
  canvas.style.transform = "scale(2.2) translate(-50%, -50%)";
  // continue the loop by recalling this function (next())
  const next = (timestamp) => {
    dt = (timestamp - previous) / 1000.0;
    // setBackgroundColor(0x6b5b95ff);
    // if (x >= 1200 - 200) {
    //   velocity *= -1;
    // } else if (x <= 0) {
    //   velocity = Math.abs(velocity);
    // }
    // // console.log(x);
    // x += velocity;
    if (i >= 240) i = 0;
    draw_shader(i);
    i++;
    // drawRectangle(x, 250, 200, 200, 0xffff00ff);
    previous = timestamp;
    ctx.putImageData(image, 0, 0);
    drawFPS(dt, width);
    window.requestAnimationFrame(next);
  };
  window.requestAnimationFrame(next);
  console.log(buffer.byteLength);
});

const drawFPS = (dt, width) => {
  ctx.font = "26px sans-serif";
  ctx.fillStyle = "#32875eff";
  ctx.fillText((1 / dt).toFixed(2), width - 80, 40);
};
