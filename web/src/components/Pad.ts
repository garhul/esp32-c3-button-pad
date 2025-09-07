
import "./Pad.css";

export type PadState = {
  buttonData: Map<number, ButtonData>,
  selected: number | null;
};

export type PadChangeListener = (st: PadState) => void;

const state: PadState = {
  buttonData: new Map<number, ButtonData>(),
  selected: null
};

export type ButtonData = {
  idx: number;
  color: string;
  mode: 'MQTT' | 'HTTP' | 'MIXED';
  topic: string | null;
  payload: string;
};

const buttonData = new Map<string, ButtonData>();
let selected: number | null = null;

// click handler
const listeners: PadChangeListener[] = [];

async function handleClick(ev: MouseEvent) {
  const currentTarget = (ev.currentTarget! as HTMLDivElement);

  if (currentTarget === null) {
    console.error('unable to retrieve button index');
    return;
  }

  const idx = parseInt(currentTarget.getAttribute('idx') ?? '-1');
  const data = await getButtonData(idx);


  if (state.selected !== null) {
    //remove selected class of old selection
    const old_selection = document.querySelector<HTMLDivElement>(`#pad > div:nth-child(${state.selected + 1})`);
    old_selection?.classList.remove('selected');
  }

  state.buttonData.set(idx, data);
  state.selected = idx;
  currentTarget.classList.add('selected');

  //update selected with loaded state



  listeners.forEach(l => l.call(null, data));
}

// Buttons box
function buildButton(idx: number) {
  const btn = document.createElement('div');

  btn.setAttribute('class', 'btn');
  btn.setAttribute('idx', `${idx}`);
  btn.addEventListener('click', handleClick);

  return btn;
}

async function getButtonData(idx: number): Promise<ButtonData> {
  //fetch('')
  const colors = ['#ff9900', '#99ff00', '#f909f0'];

  //mocked data
  return {
    idx,
    color: colors[Math.floor(Math.random() * 3)],
    mode: 'MQTT',
    topic: '/desk',
    payload: '{fx:3}'
  };

}

export function setButtonColor(idx: number, color: string) {
  const btn = document.querySelector<HTMLDivElement>(`#pad > div:nth-child(${idx + 1})`);
  if (btn === null) return false;

  console.log(`setting btn ${idx} to color ${color}`);
  btn.setAttribute('style', `background-color:${color};box-shadow: 0 0 .8em ${color}AA`);

}

export function render() {
  const root = document.querySelector<HTMLDivElement>('#pad');

  if (root === null) {
    console.error(`Pad component root element not found`);
    return;
  }

  const Buttons = Array(16).fill(0, 0, 16).map((v, idx) => buildButton(idx));
  Buttons.forEach(btn => root.appendChild(btn));
}

export function onSelectionChange(fn: PadChangeListener) {
  listeners.push(fn);
}