#pragma once

#ifndef DashWebContent_h
#define DashWebContent_h


const char TEMPLATE_CODE[] PROGMEM = R"rawliteral( 
)rawliteral";

const char HTML_INDEX_CODE[] PROGMEM = R"rawliteral( 
	<!DOCTYPE html>
	<html lang="pt-br">
	<head>
		<meta charset="UTF-8" />
		<meta name="viewport" content="width=device-width, initial-scale=1.0" />
		<title>Alimentador Automático de Peixes</title>
		<link
		href="https://fonts.googleapis.com/icon?family=Material+Icons"
		rel="stylesheet"
		/>
		<link rel="stylesheet" href="/css.css" />
	</head>
	<body>
		<header>Alimentador de peixes</header>

		<div class="content">
			<span class="material-symbols-outlined">calendar_clock</span>
			Próxima Alimentação em: <div class="next-feed-timer" id="next-feed-timer"> --:--:--</div>
		</div>
		
		<div class="content">
			<!-- img src="aqua.png" alt="Aquário" class="aquarium-image" / -->
			<button class="button" id="feed-button">
				<span class="material-icons">set_meal</span>
				Alimentar Agora
			</button>
			</div>

		<button onclick="openSettingsModal();" class="material-icons settings-icon" id="settings-icon">settings</button>
		<!-- Modal de Configurações -->
<div id="settings-modal" class="modal">
  <div class="modal-content">
    <span class="close-button">&times;</span>
    <h2>Configurações</h2>
    <div class="card">
      <h3>Tela</h3>
      <span class="material-icons toggle-icon" id="screen-toggle-icon">toggle_off</span>
    </div>
    <div class="card">
      <h3>Alimentação Automática</h3>
      <span class="material-icons toggle-icon" id="auto-feed-toggle-icon">toggle_off</span>
    </div>
  </div>
</div>
		<footer>
		<div class="version-info">Versão do App: <span class"version-value" id="version-value">1.0.0</span></div>
			<div class="activity-time" id="activity-time">Tempo de Atividade: --:--:--</div>
		</footer>

		<script src="js.js"></script>
	</body>
	</html>

)rawliteral";


const char CSS_CODE[] PROGMEM = R"rawliteral( 
body {
	font-family: Arial, sans-serif;
	margin: 0;
	padding: 0;
	background-color: #eef2f3;
}
header,
footer {
	background-color: #333;
	color: white;
	text-align: center;
	padding: 10px 0;
}
.logo {
	width: 100px; /* Ajuste conforme o tamanho do seu logo */
	height: auto;
}
.content {
	text-align: center;
	padding: 20px;
}
.aquarium-image {
	max-width: 100%;
	height: auto;
}
.button {
	display: block;
	width: 200px;
	padding: 10px;
	margin: 20px auto;
	font-size: 18px;
	cursor: pointer;
	background-color: #4caf50;
	color: white;
	border: none;
	border-radius: 5px;
}
.version-info {
	font-size: 14px;
}
.activity-time {
	font-size: 14px;
}
.material-icons {
	vertical-align: middle; /* Alinha o ícone verticalmente com o texto */
}
.settings-icon {
	position: fixed; /* Fixa o ícone na posição */
	top: 10px; /* Distância do topo da página */
	right: 10px; /* Distância da direita da página */
	font-size: 24px; /* Tamanho do ícone */
	cursor: pointer;
}
.toggle-icon.on {
	color: #4caf50; /* Cor para o estado ligado */
}
.toggle-icon.off {
	color: #f44336; /* Cor para o estado desligado */
}
.error-icon {
	color: #f44336; /* Cor do ícone de erro */
	font-size: 48px; /* Tamanho do ícone de erro */
}
.card.error {
	border: 1px solid #f44336; /* Borda do card de erro */
	padding: 20px;
	margin-top: 20px;
}



.card {
  display: flex; /* Define o layout flexível para os itens dentro do card */
  align-items: center; /* Centraliza os itens verticalmente */
  justify-content: space-between; /* Distribui o espaço entre os itens */
  padding: 10px; /* Espaçamento interno do card */
}

.toggle-icon {
  font-size: 50px; /* Tamanho do ícone de toggle */
  cursor: pointer; /* Muda o cursor para indicar que é clicável */
}


/* Estilo da Modal */
.modal {
  display: none; /* Modal escondida por padrão */
  position: fixed; /* Fica fixa na tela */
  z-index: 1; /* Fica no topo */
  left: 0;
  top: 0;
  width: 100%; /* Largura total */
  height: 100%; /* Altura total */
  overflow: auto; /* Habilita a rolagem se necessário */
  background-color: rgb(0,0,0); /* Cor de fundo */
  background-color: rgba(0,0,0,0.4); /* Cor de fundo com opacidade */
}

/* Conteúdo da Modal */
.modal-content {
  background-color: #fefefe;
  margin: 15% auto; /* 15% do topo e centralizado horizontalmente */
  padding: 20px;
  border: 1px solid #888;
  width: 50%; /* Largura da modal */
}

/* Botão de fechar */
.close-button {
  color: #aaa;
  float: right;
  font-size: 28px;
  font-weight: bold;
}

.close-button:hover,
.close-button:focus {
  color: black;
  text-decoration: none;
  cursor: pointer;
}


)rawliteral";



const char JS_CODE[] PROGMEM = R"rawliteral( 
// Função para calcular a diferença em segundos entre duas datas
function diffInSeconds(date1, date2) {
	var diff = date2.getTime() - date1.getTime();
	return Math.floor(diff / 1000);
}

// Função para formatar o tempo de atividade
function formatTime(seconds) {
	var h = Math.floor(seconds / 3600);
	var m = Math.floor((seconds % 3600) / 60);
	var s = seconds % 60;
	return (
		h.toString().padStart(2, "0") +
		":" +
		m.toString().padStart(2, "0") +
		":" +
		s.toString().padStart(2, "0")
	);
}

	
// Função para atualizar o tempo de atividade na tela
function updateActivityTime() {
  var activityTimeElement = document.getElementById("activity-time");
  var activitySeconds =
    parseInt(activityTimeElement.getAttribute("data-seconds")) || 0;
  activityTimeElement.textContent =
    "Tempo de Atividade: " + formatTime(activitySeconds);
  activityTimeElement.setAttribute("data-seconds", activitySeconds + 1);
}

// Iniciar a atualização do tempo de atividade a cada segundo
var activityInterval = setInterval(updateActivityTime, 1000);


// Função para atualizar a interface com base na resposta do endpoint /conf
function updateInterfaceWithConfig(config) {
	// Atualiza o ícone de toggle da tela
	var screenToggleIcon = document.getElementById("screen-toggle-icon");
	if ( screenToggleIcon ) {
		screenToggleIcon.textContent =
		config.TELA === "1" ? "toggle_on" : "toggle_off";
		screenToggleIcon.classList.toggle("on", config.TELA === "1");
		screenToggleIcon.classList.toggle("off", config.TELA === "0");
	}
	// Atualiza o ícone de toggle da alimentação automática
	var autoFeedToggleIcon = document.getElementById("auto-feed-toggle-icon");
	if ( autoFeedToggleIcon ){
		autoFeedToggleIcon.textContent =
		config.AutoFeed === "1" ? "toggle_on" : "toggle_off";
		autoFeedToggleIcon.classList.toggle("on", config.AutoFeed === "1");
		autoFeedToggleIcon.classList.toggle("off", config.AutoFeed === "0");
	}
	var activationTime = new Date(config.LigadoDesde.replace( /(\d{2})[-/](\d{2})[-/](\d{4})/, "$2/$1/$3") );
	// Atualiza o tempo de atividade e o horário da próxima alimentação
	document.getElementById("activity-time").textContent =
		"Tempo de Atividade: " + config.LigadoDesde;
	document.getElementById("next-feed-timer").textContent =
	"Próxima Alimentação: " + config.NextFeedAt + " | " + config.NextFeedTimeOut;
	document.getElementById("version-value").textContent = config.Version;

	var currentTime = new Date(); // Data e hora atuais
	var activitySeconds = diffInSeconds(activationTime, currentTime); // Calcula a diferença em segundos

	var activityTimeElement = document.getElementById("activity-time");
	activityTimeElement.setAttribute("data-seconds", activitySeconds);
	}
	// Função para mostrar o card de erro
	function showErrorCard() {
		clearInterval(activityInterval); // Interrompe o contador de tempo de atividade
		var contentDiv = document.querySelector(".content");
		var errorCard = document.createElement("div");
		errorCard.className = "card error";
		errorCard.innerHTML =
		'<span class="material-icons error-icon">error</span>' +
		"<p>Não foi possível obter as configurações. Por favor, tente novamente mais tarde.</p>";
		contentDiv.innerHTML = ""; // Limpa o conteúdo atual
		contentDiv.appendChild(errorCard); // Adiciona o card de erro ao conteúdo
	}

	// Função para consultar o endpoint /conf e atualizar a interface
	function fetchConfigAndUpdateInterface() {
		var xhr = new XMLHttpRequest();
		xhr.open("GET", "/conf", true);
		xhr.onload = function () {
		if (xhr.status === 200) {
		var config = JSON.parse(xhr.responseText);
		updateInterfaceWithConfig(config);
		} else {
		showErrorCard(); // Mostra o card de erro se houver uma resposta de erro
		}
		};
		xhr.onerror = function () {
			showErrorCard(); // Mostra o card de erro se houver um erro de conexão
		};
		xhr.send();
	}

	// Inicializa a interface com as configurações atuais
	fetchConfigAndUpdateInterface();

	// Consultar o dispositivo a cada minuto
	setInterval(fetchConfigAndUpdateInterface, 60000);


		
// Função para abrir a modal de configurações
function openSettingsModal() {
	document.getElementById("settings-modal").style.display = "block";
}

// Função para fechar a modal de configurações
function closeSettingsModal() {
	document.getElementById("settings-modal").style.display = "none";
}

// Evento para o botão de fechar na modal
document
	.querySelector(".close-button")
	.addEventListener("click", closeSettingsModal);

document.getElementById("feed-button").addEventListener("click", function () {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", "/a", true);
  xhr.onload = function () {
    if (xhr.status !== 200) {

      showErrorCard(); // Mostra o card de erro se houver uma resposta de erro
    }
  };
  xhr.onerror = function () {
    showErrorCard(); // Mostra o card de erro se houver um erro de conexão
  };
  xhr.send();
});


// Função para atualizar o ícone de toggle da tela
function updateScreenToggleIcon(state) {
  var screenToggleIcon = document.getElementById('screen-toggle-icon');
  if (state === '1') { // Se o estado for '1', mostra como ligado
    screenToggleIcon.textContent = 'toggle_on';
    screenToggleIcon.classList.add('on');
    screenToggleIcon.classList.remove('off');
  } else { // Se o estado for '0', mostra como desligado
    screenToggleIcon.textContent = 'toggle_off';
    screenToggleIcon.classList.add('off');
    screenToggleIcon.classList.remove('on');
  }
}

// Evento para o ícone de toggle da tela
document.getElementById('screen-toggle-icon').addEventListener('click', function() {
  var currentState = this.classList.contains('on') ? '1' : '0';
  var endpoint = currentState === '1' ? '/tOFF' : '/tON'; // Determina o endpoint correto

  // Faz uma requisição AJAX para o endpoint
  var xhr = new XMLHttpRequest();
  xhr.open('POST', endpoint, true);
  xhr.onload = function() {
    if (xhr.status === 200) {
      // Atualiza o ícone de toggle com a resposta do endpoint
      updateScreenToggleIcon(xhr.responseText);
    } else {
      // Trata o erro
      console.error('Erro ao tentar alterar o estado da tela.');
    }
  };
  xhr.onerror = function() {
    // Trata o erro de conexão
    console.error('Erro de conexão ao tentar alterar o estado da tela.');
  };
  xhr.send();
});

// Função para atualizar o ícone de toggle da alimentacao automatica
function updateFeedToggleIcon(state) {
  var feedToggleIcon = document.getElementById('auto-feed-toggle-icon');
  if (state === '1') { // Se o estado for '1', mostra como ligado
    feedToggleIcon.textContent = 'toggle_on';
    feedToggleIcon.classList.add('on');
    feedToggleIcon.classList.remove('off');
  } else { // Se o estado for '0', mostra como desligado
    feedToggleIcon.textContent = 'toggle_off';
    feedToggleIcon.classList.add('off');
    feedToggleIcon.classList.remove('on');
  }
}

// Evento para o ícone de toggle da alimentacao automatica
document.getElementById('auto-feed-toggle-icon').addEventListener('click', function() {
  var currentState = this.classList.contains('on') ? '1' : '0';
  var endpoint = currentState === '0' ? '/aON' : '/aOFF';

  // Faz uma requisição AJAX para o endpoint
  var xhr = new XMLHttpRequest();
  xhr.open('POST', endpoint, true);
  xhr.onload = function() {
    if (xhr.status === 200) {
      // Atualiza o ícone de toggle com a resposta do endpoint
      updateFeedToggleIcon(xhr.responseText);
    } else {
      // Trata o erro
      console.error('Erro ao tentar alterar o estado da alimentação automática.');
    }
  };
  xhr.onerror = function() {
    // Trata o erro de conexão
    console.error('Erro de conexão ao tentar alterar o estado da alimentação automática.');
  };
  xhr.send();
});





)rawliteral";
#endif // DashWebContent_H
