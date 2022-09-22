import { OutilCalorie } from "./outilCalorie";

export class OutilPauvre extends OutilCalorie {
	#table = new Map<string, number>([
		["pomme", 50],
		["banane", 60],
	]);

	getState(): Map<string, number> {
		return this.#table;
	}

	setState(state: Map<string, number>): void {
		this.#table = state;
		this.notify();
	}

	change() {
		this.setState(new Map([...this.#table, ["pomme", Math.random() * 100]]));
	}
}
