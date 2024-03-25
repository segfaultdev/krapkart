window.my_canvas = document.getElementById("mycanvas");
window.context = my_canvas.getContext("2d");

window.my_canvas.style.imageRendering = "crisp-edges";

window.context.imageSmoothingEnabled = false;
window.context.mozImageSmoothingEnabled = false;
window.context.oImageSmoothingEnabled = false;
window.context.webkitImageSmoothingEnabled = false;
window.context.msImageSmoothingEnabled = false;

let mouse_locked = false;

let send_lock_delta = function(event) {
  window.send_event("lock_delta", [event.movementX, event.movementY]);
};

document.onpointerlockchange = function() {
  if (mouse_locked) {
    document.removeEventListener("mousemove", send_lock_delta, false);
    mouse_locked = false;
    
    window.send_event("lock_off", []);
  } else {
    document.addEventListener("mousemove", send_lock_delta, false);
    mouse_locked = true;
    
    window.send_event("lock_on", []);
  }
};
