import { Kafka } from "kafkajs";
import { stdin } from "process";
import { createInterface } from "readline";

const kafka = new Kafka({
  clientId: "bun-producer",
  brokers: ["localhost:9092"],
});

const producer = kafka.producer();

const rl = createInterface({
  input: stdin,
  output: process.stdout,
  prompt: "Escribe un mensaje (Ctrl+C para salir): ",
});

const run = async () => {
  await producer.connect();
  console.log("Productor conectado. Escribe tus mensajes...");

  rl.prompt();

  rl.on("line", async (line) => {
    try {
      await producer.send({
        topic: "quickstart-events",
        messages: [{ value: line }],
      });

      console.log(`Mensaje enviado: ${line}`);
      rl.prompt();
    } catch (err) {
      console.error("Error enviando mensaje:", err);
    }
  }).on("close", async () => {
    console.log("\nCerrando productor...");
    await producer.disconnect();
    process.exit(0);
  });
};

run().catch(console.error);
