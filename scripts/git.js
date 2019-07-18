function display_repos(response) {
  var e = document.getElementById("root");
  e.innerHTML = response;
}

function update() {

  var xhr = new XMLHttpRequest();
  xhr.open("POST", "/", true);
  xhr.setRequestHeader("content-Type", "application/json");
  xhr.send(JSON.stringify({
    key: "value"
  }));

  xhr.onload = function() {
    display_repos(this.responseText);
  }
}

update();
