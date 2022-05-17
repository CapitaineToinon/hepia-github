import { Fruit } from "./Fruit";
import { FruitType } from "./FruitType";

export class Pomme extends Fruit {
	constructor({ type }: { type: FruitType }) {
		super({
			type,
			nom: "pomme",
			pepin: true,
		});
	}
}
