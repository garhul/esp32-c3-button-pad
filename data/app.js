
function updateUI(state) {

  if (state.br !== parseInt(_id("br").value)) {
    _id("br").value = state.br;
  }

  if (state.spd !== parseInt(_id("spd").value)) {
    _id("spd").value = state.spd;
  }

  let btns = document.getElementsByTagName("Button");
  for (var btn of btns) {
    if (state.mode === 2) {
      _id("playPause").innerHTML = "Pause"
      _id("playPause").attributes.cmd.value = "pause";
      if (state.fx === parseInt(btn.attributes.payload.value)) {
        btn.classList.remove("btn-outline-secondary");
        btn.classList.add("btn-outline-success");
      } else {
        btn.classList.remove("btn-outline-success");
        btn.classList.add("btn-outline-secondary");
      }
    } else if (state.mode === 0) {
      _id("playPause").innerHTML = "Play"
      _id("playPause").attributes.cmd.value = "play";
      btn.classList.remove("btn-outline-success");
      btn.classList.add("btn-outline-secondary");
    } else if (state.mode === 1) {
      _id("playPause").innerHTML = "Play";
      _id("playPause").attributes.cmd.value = "play";
      btn.classList.remove("btn-outline-success");
      btn.classList.add("btn-outline-secondary");
    }
  }

}

function loadState() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", '/state', true)
  xhr.onreadystatechange = function () { // Call a function when the state changes.
    if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
      var rsp = JSON.parse(this.response);
      updateUI(rsp);
      console.log(rsp);

    } else {
      console.log('error on communication :', this);
    }
  }
  xhr.send();
}

function _send(cmd, payload) {
  if (in_use) return;
  console.info(`sending cmd:${cmd}, payload: ${payload}`);
  in_use = true;

  var xhr = new XMLHttpRequest();
  xhr.open("POST", '/cmd', true);
  xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");

  xhr.onreadystatechange = function () { // Call a function when the state changes.
    if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
      console.log(this);
      var rsp = JSON.parse(this.response);
      updateUI(rsp);
      in_use = false;
    } else if (this.readyState === XMLHttpRequest.DONE && this.status !== 200) {
      console.log('error on communication :', this);
      in_use = false;
    }
  }
  xhr.send(`cmd=${cmd}&payload=${payload}`);
}

function loadInfo() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", '/info', true)
  xhr.onreadystatechange = function () { // Call a function when the state changes.
    if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
      var rsp = JSON.parse(this.response);
      console.log(rsp);
      _id("ip").innerHTML = rsp.ip;
      _id("name").innerHTML = rsp.human_name;
    } else if (this.readyState === XMLHttpRequest.DONE && this.status !== 200) {
      console.log('error on communication :', this);
    }
  }
  xhr.send();
}

function validatePayload(payload) {

}


function getButtonEl(idx) {
  const btn = document.createElement('div');

  btn.setAttribute('class', 'btn');
  btn.setAttribute('idx', idx);
  btn.addEventListener('click', (ev) => {
    console.log(ev);
  });


  return btn;
}



// Buttons box
const Buttons = Array(16).fill(0, 0, 16).map((v, idx) => getButtonEl(idx));
function buttonBox(rootEl) {
  Buttons.forEach(btn => rootEl.appendChild(btn));
}


// Draw color picker

function colorPicker(rootEl) {
  const colors = Array(64).fill(0, 0, 64).map((v, idx) => {
    const s = document.createElement('span');
    s.innerHTML = '⬢';
    s.setAttribute('class', 'colorHexagon');
    const hue = idx * (360 / 64);
    s.setAttribute('style', `color:hsl(${hue}, 80%,50%)`);
    s.addEventListener('click', (e) => {
      console.log(e);
    });

    rootEl.appendChild(s);
  });

}

// Draw buttons box
buttonBox(document.getElementById('pad'));
colorPicker(document.getElementById('colorPicker'));