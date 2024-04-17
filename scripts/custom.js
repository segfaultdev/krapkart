window.my_canvas = document.getElementById("mycanvas");
window.context = my_canvas.getContext("2d", {alpha: false});

window.my_canvas.style.imageRendering = "crisp-edges";

window.context.imageSmoothingEnabled = false;
window.context.mozImageSmoothingEnabled = false;
window.context.oImageSmoothingEnabled = false;
window.context.webkitImageSmoothingEnabled = false;
window.context.msImageSmoothingEnabled = false;

window.rect_color = function(x, y, width, height, red, green, blue) {
  window.context.fillStyle = "#" + ((red << 16) | (green << 8) | blue).toString(16).padStart(6, "0");
  window.context.fillRect(x, y, width, height);
};

var send_lock_delta = function(event) {
  window.send_event("lock_delta", [event.movementX, event.movementY]);
};

document.onpointerlockchange = function() {
  if (window.mouse_locked) {
    document.removeEventListener("mousemove", send_lock_delta, false);
    window.mouse_locked = false;
    
    window.send_event("lock_off", []);
  } else {
    document.addEventListener("mousemove", send_lock_delta, false);
    window.mouse_locked = true;
    
    window.send_event("lock_on", []);
  }
};

var animate_id = null;

if (!window.true_clearInterval) {
  window.true_clearInterval = window.clearInterval;
}

if (!window.true_setInterval) {
  window.true_setInterval = window.setInterval;
}

window.clearInterval = function(id) {
  // console.log("window.clearInterval");
  
  if (id == "segsyboi") {
    if (animate_id) {
      window.cancelAnimationFrame(animate_id);
    }
  } else {
    window.true_clearInterval();
  }
};

window.setInterval = function(callback, target_ms) {
  // console.log("window.setInterval -> " + target_ms + "ms");
  
  if (target_ms == 256) {
    callback();
  } else if (target_ms == 32) {
    if (animate_id) {
      window.cancelAnimationFrame(animate_id);
      animate_id = null;
    }
    
    let callback_wrapper = function(delta) {
      callback();
      animate_id = window.requestAnimationFrame(callback_wrapper);
    };
    
    animate_id = window.requestAnimationFrame(callback_wrapper);
  } else {
    return window.true_setInterval(callback, target_ms);
  }
  
  return "segsyboi";
};
