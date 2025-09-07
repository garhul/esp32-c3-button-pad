import { ButtonData } from "./components/Pad";

export function getButtonData(idx: number): ButtonData {
  console.log(`retrieving btn ${idx}`);

  return {
    idx,
    color: '#ff9900',
    mode: 'MQTT',
    topic: '/desk',
    payload: '{fx:3}'
  };
}


export function setButtonData(idx: number, data: ButtonData) {
  console.dir({ idx, data });
}