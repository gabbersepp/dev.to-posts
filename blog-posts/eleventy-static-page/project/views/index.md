# Display tweets

**Tweets**
{% for t in collections.tweets %}
* {{ t.id }} - {{ t.title }} - {{ t.descr }}
{% endfor %}
