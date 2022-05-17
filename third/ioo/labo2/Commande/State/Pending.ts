import { Commande } from "../index";
import { Paid } from "./Paid";
import { State } from "./State";

export class Pending extends State {
	traiterCommande(commande: Commande) {
		console.log("commande was paid!");
		commande.state = new Paid();
	}
}
