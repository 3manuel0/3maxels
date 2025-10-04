let wasm;
let canvas;
let ctx;
let previous;
let dt;
let blured = false;
const str_len = wasmlib.str_len;
const get_str = wasmlib.get_str;

WebAssembly.instantiateStreaming(fetch("game.wasm"), {
  env: wasmlib.make_environment({
    printf: wasmlib.printf,
  }),
}).then((w) => {
  wasm = w;
  canvas = document.getElementById("canvas");
  ctx = canvas.getContext("2d");
  const { setBackgroundColor, change_buffer, drawRectangle } =
    w.instance.exports;
  const buffer = wasm.instance.exports.memory.buffer;
  let buff = new Uint8ClampedArray(
    buffer,
    setBackgroundColor(0x6b5b95ff), //rgba
    1200 * 700 * 4
  );
  console.log(setBackgroundColor(0x6b5b95ff));
  drawRectangle(20, 10, 350, 10, 0xffff00ff);
  const image = new ImageData(buff, 1200, 700);
  // change_buffer(buffer_ptr());
  let previous;
  const next = (timestamp) => {
    // continue the loop by recalling this function (next())
    dt = (timestamp - previous) / 1000.0;
    // change_buffer();
    previous = timestamp;
    ctx.putImageData(image, 0, 0);
    drawFPS(dt);
    window.requestAnimationFrame(next);
  };
  window.requestAnimationFrame(next);
  console.log(buffer.byteLength);
});

const drawFPS = (dt) => {
  ctx.font = "30px sans-serif";
  ctx.fillText("" + 1 / dt, 900, 40);
};
