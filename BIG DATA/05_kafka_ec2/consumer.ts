import { Kafka } from "kafkajs";

const kafka = new Kafka({
  clientId: "bun-consumer",
  brokers: ["localhost:9092"],
});

const consumer = kafka.consumer({ groupId: "bun-group" });

const run = async () => {
  await consumer.connect();
  console.log("Consumidor conectado");

  await consumer.subscribe({ topic: "quickstart-events", fromBeginning: true });

  await consumer.run({
    eachMessage: async ({ topic, partition, message }) => {
      console.log(`Recibido: ${message.value.toString()}`);
    },
  });
};

run().catch(console.error);
