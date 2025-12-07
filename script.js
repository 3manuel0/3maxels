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
  }),
}).then((w) => {
  wasm = w;
  canvas = document.getElementById("canvas");
  ctx = canvas.getContext("2d");
  const { setBackgroundColor, change_buffer, drawRectangle, getBuffer } =
    w.instance.exports;
  const buffer = wasm.instance.exports.memory.buffer;
  let bg_color = 0x6b5b95ff;
  //0xff955b6b

  let buff = new Uint8ClampedArray(
    buffer,
    setBackgroundColor(0x6b5b95ff), //rgba
    1200 * 700 * 4
  );
  // console.log(setBackgroundColor(0x6b5b95ff));
  console.log(new Uint8ClampedArray(buffer, getBuffer(), 1200 * 700 * 4));
  ddrawRectangle = drawRectangle;
  // drawRectangle(300, 400, 400, 200, 0xffff00ff);
  const image = new ImageData(buff, 1200, 700);
  // change_buffer(buffer_ptr());
  let previous;
  let x = 0;
  let velocity = 6;
  const next = (timestamp) => {
    // continue the loop by recalling this function (next())
    dt = (timestamp - previous) / 1000.0;
    setBackgroundColor(0x6b5b95ff);
    drawRectangle(x, 250, 200, 200, 0xffff00ff);
    console.log(x);
    if (x > 1200 - 200) {
      velocity *= -1;
    } else if (x < 0) {
      velocity *= -1;
    }
    x += velocity;
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
