function connection_test()
{           
    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'http://127.0.0.1:6780/test', true);
    xhr.send();
    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 202) {
            console.log('请求成功');
            alert("请求成功" + xhr.responseText);
        } else {
            console.log('请求失败，状态码：' + xhr.status);
        }
    }};
}

function displayDate()
{
    document.getElementById("datetest").innerHTML=Date();
}
