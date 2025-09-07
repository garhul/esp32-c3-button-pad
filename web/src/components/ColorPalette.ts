
import './ColorPalette.css';

const COLORS = 64;

export function render() {
  const rootEl = document.querySelector('#colorPalette');

  const colors = Array(COLORS).fill(0, 0, COLORS).map((v, idx) => {
    const s = document.createElement('span');
    s.innerHTML = '⬢';
    s.setAttribute('class', 'colorHexagon');
    const hue = idx * (360 / 64);
    s.setAttribute('style', `color:hsl(${hue}, 80%,50%)`);
    s.setAttribute('colorVal', 'coso');
    s.addEventListener('click', (e: MouseEvent) => {
      const target = (e.currentTarget! as HTMLDivElement);
      console.log(target.getAttribute('colorVal'));
    });
    rootEl?.appendChild(s);
  });
}