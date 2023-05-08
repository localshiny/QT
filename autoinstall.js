new QWebChannel(qt.webChannelTransport, function (channel) {
 var e = document.getElementsByName("copylink");
//     alert("uuuuuuuuuuu");
 for(var i = 0; i < e.length; ++i)
 {
     var item = e[i];
     if (item.onclick != null)
     {
        item.addEventListener("click", function(event){
            console.log(event.target.id);
            channel.objects.qt_autoInstall.on_install_handle(event.target.id);
        });
     }
 }
 var d = document.getElementsByName("localshiny_delete_app");
 for(var i = 0; i < d.length; ++i)
 {
     var item = d[i];
     if (item.onclick != null)
     {
        item.addEventListener("click", function(event){
                console.log(event.target.id);
                channel.objects.qt_autoDelete.on_delete_handle(event.target.id);
     });
     }
 }
 var r = document.getElementsByName("localshiny_run_app");
 for(var i = 0; i < r.length; ++i)
 {
     var item = r[i];
     if (item.onclick != null)
     {
        item.addEventListener("click", function(event){
            console.log(event.target.id);
            channel.objects.qt_autoRun.on_run_handle(event.target.id);
        });
     }
 }
 var mySpace=document.getElementById("mySpace");
 if (mySpace!=null)
 {
    mySpace.addEventListener("click", function(event){
        channel.objects.qt_autoOpenMySpace.on_open_my_space_handle();
 });
 }
}
)
