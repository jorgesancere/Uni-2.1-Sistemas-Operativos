# SO <mark>D</mark> 

PRÁTICA 3 <u>SD</u> 

## <u>P3. APLICACIÓN DISTRIBUIDA</u> 

### **PREDICCIÓN DE CONSUMO ELÉCTRICO CON ARQUITECTURA DISTRIBUIDA** 

En esta práctica construiremos una aplicación distribuida que permite predecir el consumo eléctrico en un edificio de la Universidad de Alicante. 

El proyecto se desarrollará en Node.js, empleando una arquitectura de microservicios REST y almacenamiento en MongoDB. 

El objetivo no es solo obtener una predicción, sino aprender a diseñar y desplegar un sistema distribuido real, donde varios servicios cooperan y se comunican a través de la red siguiendo el modelo TCP/IP. 

#### JUSTIFICACIÓN TECNOLÓGICA 

En esta sección se describen las decisiones tecnológicas adoptadas para el desarrollo del proyecto y las razones por las que son adecuadas para un entorno docente y profesional. 

Node.js 

Node.js será la base de todos los microservicios del proyecto. 

Se elige por las siguientes razones: 

- **Lenguaje único y actual** : utiliza JavaScript, uno de los lenguajes más extendidos y demandados, tanto en frontend como en backend. Esto simplifica el aprendizaje y la integración con la interfaz web final. 

- **Modelo asíncrono y no bloqueante** : está diseñado para manejar muchas peticiones simultáneas con gran eficiencia, lo que lo hace ideal para sistemas distribuidos y APIs que deben comunicarse entre sí constantemente. 

- **Ecosistema maduro** : existen miles de librerías y herramientas (como Express, Mongoose o TensorFlow.js) que facilitan construir servicios REST, conectar con bases de datos y ejecutar modelos de predicción. 

- **Facilidad de despliegue** : es ligero, rápido y funciona perfectamente dentro de contenedores Docker. 

En definitiva, Node.js permite construir microservicios modernos, escalables y mantenibles con una curva de aprendizaje asequible. 

#### Arquitectura de microservicios 

El sistema se diseñará siguiendo el estilo de microservicios, donde cada componente tiene una función bien definida y se comunica con los demás mediante peticiones de red. 

Este enfoque se adopta porque: 

- **Favorece la modularidad** : cada servicio se centra en una tarea (obtener datos, procesarlos, realizar predicciones o servir la interfaz). 

- **Facilita el mantenimiento** : los cambios o errores en un módulo no afectan al resto del sistema. 

- **Permite desplegar y escalar servicios de forma independiente** , añadiendo instancias solo donde se necesiten. 

- **Reproduce el funcionamiento de sistemas reales** en la industria, donde los distintos componentes de una aplicación (bases de datos, servicios de autenticación, lógica de negocio, API pública, etc.) están distribuidos en red. 

El objetivo didáctico es comprender cómo se divide y orquesta una aplicación compleja en varios servicios autónomos que colaboran entre sí. 

Esta arquitectura se materializará en varios servicios REST implementados en Node.js, que cooperarán entre sí para generar y servir las predicciones. 

Estilo REST (Representational State Transfer) 

Los microservicios se comunicarán utilizando API REST, un estándar ampliamente adoptado para el intercambio de información entre sistemas distribuidos. 

REST se basa en los principios del protocolo HTTP y aporta varias ventajas: 

- **Sencillez** : usa métodos HTTP estándar (GET, POST, PUT, DELETE) y mensajes en formato JSON. 

- **Interoperabilidad** : cualquier lenguaje o cliente puede consumir las APIs REST sin dependencias especiales. 

- **Escalabilidad** : las operaciones REST suelen ser _sin estado_ (stateless), lo que simplifica el escalado horizontal y la recuperación ante fallos. 

- **Compatibilidad con el modelo TCP/IP** : cada comunicación se realiza mediante peticiones HTTP sobre TCP, lo que permite observar y analizar fácilmente las capas de red en acción. 

REST es hoy el patrón de comunicación más usado en microservicios, por lo que dominarlo es clave para comprender tanto los fundamentos teóricos de las redes como las prácticas de desarrollo modernas. 

#### Entrenamiento offline del modelo de predicción 

El componente de inteligencia artificial del sistema se basa en un modelo de red neuronal MLP entrenado previamente fuera del entorno Node.js. 

El modelo de aprendizaje automático que se utilizará para realizar las predicciones se entrenará de forma offline, utilizando Python y el entorno Kaggle. 

Esta decisión responde a criterios tanto técnicos como pedagógicos, y busca reproducir el flujo de trabajo real en el desarrollo de sistemas inteligentes distribuidos. 

Entrenar el modelo fuera del sistema distribuido permite separar dos fases con objetivos y requisitos diferentes: 

- **Fase de entrenamiento** : requiere un entorno con librerías científicas avanzadas, gran capacidad de cálculo y herramientas de experimentación (por ejemplo, TensorFlow, Keras, scikit-learn, NumPy…). 

- **Fase de predicción (inferencia)** : se centra en ofrecer respuestas rápidas y estables dentro de un servicio web en producción. 

Esta separación refleja la práctica profesional habitual, en la que los científicos de datos entrenan los modelos en entornos especializados, y los desarrolladores los despliegan posteriormente en sistemas de producción. 

2 

Además, entrenar offline simplifica el desarrollo de los microservicios y evita sobrecargar la infraestructura de predicción con tareas de cálculo intensivo. 

Por qué usar Python 

Python es el lenguaje estándar en el ámbito del aprendizaje automático y la ciencia de datos. 

Se elige por las siguientes razones: 

- Dispone de librerías especializadas (TensorFlow, Keras, scikit-learn, pandas, NumPy) que facilitan el entrenamiento y evaluación de modelos. 

- Tiene una comunidad muy amplia y activa, lo que garantiza soporte, documentación y recursos didácticos. 

- Su sintaxis sencilla y su enfoque modular lo hacen ideal para que el alumnado comprenda las fases del proceso de modelado. 

- Permite exportar fácilmente modelos entrenados al formato TensorFlow.js, que podrá ser utilizado por los microservicios en Node.js. 

En definitiva, Python ofrece el entorno más completo, estable y accesible para desarrollar y experimentar con modelos de Machine Learning. 

Por qué usar el entorno Kaggle 

Kaggle es una plataforma en la nube gratuita que permite ejecutar notebooks de Python con todo el entorno ya configurado. 

Se utiliza en esta práctica por los siguientes motivos: 

- **Sin necesidad de instalación** : el entorno ya incluye Python, TensorFlow, scikit-learn y GPU opcional, evitando problemas de configuración local. 

- **Interfaz tipo notebook** : permite mezclar explicación, código y resultados en un único documento, favoreciendo el aprendizaje paso a paso. 

- **Exportación directa de modelos** : se pueden guardar los ficheros del modelo (.h5 o model_tfjs/) listos para usar en los microservicios. 

De esta manera, Kaggle simplifica la parte técnica y permite centrarse en **comprender el proceso de entrenamiento y validación** , en lugar de en los problemas de instalación o compatibilidad de librerías. 

Conexión con el sistema distribuido 

El modelo entrenado en Kaggle se exportará al formato TensorFlow.js, lo que permite su uso dentro de los servicios desarrollados en Node.js. 

Así, el flujo completo será el mismo que en un entorno real: 

1. **Entrenamiento offline** en Python (Kaggle). 

2. **Exportación del modelo** (onnx). 

3. **Carga e inferencia** del modelo en Node.js dentro del microservicio de predicción. 

Este enfoque permite entender cómo los modelos de Machine Learning se integran en arquitecturas distribuidas modernas, combinando herramientas de ciencia de datos y desarrollo de servicios web. 

#### Conclusión 

El uso conjunto de Node.js, una arquitectura de microservicios y comunicación mediante REST nos permitirá: 

- Trabajar con tecnologías actuales y relevantes en el mercado. 

- Aplicar en un entorno práctico los conceptos teóricos de sistemas distribuidos y protocolos de red. 

- Entender cómo se diseñan, comunican y despliegan los componentes de una aplicación distribuida real. 

3 

Este enfoque equilibra aprendizaje técnico y comprensión conceptual, formando una visión completa de cómo funcionan las aplicaciones modernas conectadas en red. 

El entrenamiento offline en Python (Kaggle) es la mejor opción porque: 

- Permite aprovechar el ecosistema científico de Python sin complicaciones de instalación. 

- Separa correctamente las fases de entrenamiento e inferencia, reflejando un escenario profesional real. 

- Facilita la exportación del modelo a TensorFlow.js y su integración con los microservicios en Node.js. 

- Simplifica la logística docente y asegura la reproducibilidad de los resultados. 

Este diseño permite trabajar con las **mejores herramientas de cada ámbito** : Python para entrenar y Node.js para desplegar, integrando ambos mundos en una misma práctica. 

## METODOLOGÍA 

La práctica se desarrollará siguiendo la metodología SCRUM. Las diferentes tareas relacionadas con los sprint semanales están en el tablero público de trello: https://trello.com/b/blyKkUPy/practica-3-sd-2026 

En cada clase presencial se darán las instrucciones para llevar a cabo las tareas del sprint de la semana. A lo largo de la práctica se irán aportando materiales complementarios para que los estudiantes puedan ir realizando las tareas. 

## EVALUACIÓN 

- Se entregarán el o los enlaces de proyectos en github 

- La evaluación se realizará el día 18 de diciembre en la clase de prácticas. 

4 

