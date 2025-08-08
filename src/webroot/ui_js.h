const char ui_js[] PROGMEM = R"rawliteral(
(function (window, document) {

    // we fetch the elements each time because docusaurus removes the previous
    // element references on page navigation
    function getElements() {
        return {
            layout: document.getElementById('layout'),
            menu: document.getElementById('menu'),
            menuLink: document.getElementById('menuLink')
        };
    }

    function toggleClass(element, className) {
        var classes = element.className.split(/\s+/);
        var length = classes.length;
        var i = 0;

        for (; i < length; i++) {
            if (classes[i] === className) {
                classes.splice(i, 1);
                break;
            }
        }
        // The className is not found
        if (length === classes.length) {
            classes.push(className);
        }

        element.className = classes.join(' ');
    }

    function toggleAll() {
        var active = 'active';
        var elements = getElements();

        toggleClass(elements.layout, active);
        toggleClass(elements.menu, active);
        toggleClass(elements.menuLink, active);
    }
    
    function handleEvent(e) {
        var elements = getElements();
        
        if (e.target.id === elements.menuLink.id) {
            toggleAll();
            e.preventDefault();
        } else if (elements.menu.className.indexOf('active') !== -1) {
            toggleAll();
        }
    }
    
    document.addEventListener('click', handleEvent);

}(this, this.document));

$(document).ready(function () {
    $('#main').append(systemstatus);
    //handle menu clicks
    $(document).on('click', 'ul.pure-menu-list li a', function (event) {
        var page = $(this).attr('href');
        console.log("click catched: " + page);
        update_page(page);
        $('li.pure-menu-item').removeClass("pure-menu-selected");
        $(this).parent().addClass("pure-menu-selected");
        event.preventDefault();
    });
    $(document).on('click', '#headingindex', function (event) {
        update_page('index');
        $('li.pure-menu-item').removeClass("pure-menu-selected");
        event.preventDefault();
    });
    //Request data from server
});

//function to load html main content
var lastPageReq = "";
function update_page(page) {
    lastPageReq = page;
    $('#main').empty();
    $('#main').css('max-width', '768px')
    if (page == 'test_html') { $('#main').append(test_html); }
}

)rawliteral";
