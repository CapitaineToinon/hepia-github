import { Commande } from "../index";
import { Sent } from "./Sent";
import { State } from "./State";

export class Paid extends State {
	traiterCommande(commande: Commande) {
		console.log("Commande was sent!");
		commande.state = new Sent();
	}
}
