import { Fruit } from "./Fruit";
import { FruitType } from "./FruitType";

export class Banane extends Fruit {
	constructor({ type }: { type: FruitType }) {
		super({
			type,
			nom: "banane",
			pepin: false,
		});
	}
}
