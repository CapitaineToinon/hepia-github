import { Fruit } from "./Fruit/fruit";
import { OutilPauvre } from "./Fruit/Outil/outilPauvre";

async function main() {
	const outil = new OutilPauvre();

	new Fruit({
		name: "pomme",
		outil,
	});

	for (let i = 0; i < 5; i++) {
		outil.change();
		await new Promise((r) => setTimeout(r, 1000));
	}
}

main();
